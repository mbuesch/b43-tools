/*
 *   Copyright (C) 2006-2007  Michael Buesch <mb@bu3sch.de>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2
 *   as published by the Free Software Foundation.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 */

#include "main.h"
#include "list.h"
#include "util.h"
#include "parser.h"
#include "args.h"
#include "initvals.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


extern int yyparse(void);
extern int yydebug;

struct file infile;
const char *infile_name;
const char *outfile_name;


struct out_operand {
	enum {
		OUTOPER_NORMAL,
		OUTOPER_LABELREF,
	} type;

	union {
		unsigned int operand; /* For NORMAL */
		struct label *label; /* For LABELREF */
	} u;
};

struct code_output {
	enum {
		OUT_INSN,
		OUT_LABEL,
	} type;

	/* Set to true, if this is a jump instruction.
	 * This is only used when assembling RET to check
	 * whether the previous instruction was a jump or not. */
	bool is_jump_insn;

	unsigned int opcode;
	struct out_operand operands[3];

	/* The absolute address of this instruction.
	 * Only used in resolve_labels(). */
	unsigned int address;

	const char *labelname; /* only for OUT_LABEL */
	/* Set to 1, if this is the %start instruction. */
	int is_start_insn;

	struct list_head list;
};

struct assembler_context {
	/* The architecture version (802.11 core revision) */
	unsigned int arch;

	struct label *start_label;

	/* Tracking stuff */
	struct statement *cur_stmt;

	struct list_head output;
};


#define for_each_statement(ctx, s)			\
	list_for_each_entry(s, &infile.sl, list) {	\
		ctx->cur_stmt = s;

#define for_each_statement_end(ctx, s)			\
	} do { ctx->cur_stmt = NULL; } while (0)

#define _msg_helper(type, stmt, msg, x...)	do {		\
	fprintf(stderr, "Assembler " type);			\
	if (stmt) {						\
		fprintf(stderr, " (file \"%s\", line %u)",	\
			stmt->info.file,			\
			stmt->info.lineno);			\
	}							\
	fprintf(stderr, ":\n  " msg "\n" ,##x);			\
						} while (0)

#define asm_error(ctx, msg, x...)	do {			\
	_msg_helper("ERROR", (ctx)->cur_stmt, msg ,##x);	\
	exit(1);						\
					} while (0)

#define asm_warn(ctx, msg, x...)	\
	_msg_helper("warning", (ctx)->cur_stmt, msg ,##x)

#define asm_info(ctx, msg, x...)	\
	_msg_helper("info", (ctx)->cur_stmt, msg ,##x)


static void eval_directives(struct assembler_context *ctx)
{
	struct statement *s;
	struct asmdir *ad;
	struct label *l;
	int have_start_label = 0;
	int have_arch = 0;

	for_each_statement(ctx, s) {
		if (s->type == STMT_ASMDIR) {
			ad = s->u.asmdir;
			switch (ad->type) {
			case ADIR_ARCH:
				if (have_arch)
					asm_error(ctx, "Multiple %%arch definitions");
				ctx->arch = ad->u.arch;
				if (ctx->arch != 5 && ctx->arch != 15) {
					asm_error(ctx, "Architecture version %u unsupported",
						  ctx->arch);
				}
				have_arch = 1;
				break;
			case ADIR_START:
				if (have_start_label)
					asm_error(ctx, "Multiple %%start definitions");
				ctx->start_label = ad->u.start;
				have_start_label = 1;
				break;
			default:
				asm_error(ctx, "Unknown ASM directive");
			}
		}
	} for_each_statement_end(ctx, s);

	if (!have_arch)
		asm_error(ctx, "No %%arch defined");
	if (!have_start_label)
		asm_info(ctx, "Using start address 0");
}

static bool is_possible_imm(unsigned int imm)
{
	unsigned int mask;

	/* Immediates are only possible up to 16bit (wordsize). */
	mask = ~0;
	mask <<= 16;
	if (imm & (1 << 15)) {
		if ((imm & mask) != mask &&
		    (imm & mask) != 0)
			return 0;
	} else {
		if ((imm & mask) != 0)
			return 0;
	}

	return 1;
}

static bool is_valid_imm(struct assembler_context *ctx,
			 unsigned int imm)
{
	unsigned int mask;
	unsigned int immediate_size;

	/* This function checks if the immediate value is representable
	 * as a native immediate operand.
	 *
	 * For v5 architecture the immediate can be 10bit long.
	 * For v15 architecture the immediate can be 11bit long.
	 *
	 * The value is sign-extended, so we allow values
	 * of 0xFFFA, for example.
	 */

	if (!is_possible_imm(imm))
		return 0;
	imm &= 0xFFFF;

	if (ctx->arch == 5) {
		immediate_size = 10; /* 10bit */
	} else if (ctx->arch == 15) {
		immediate_size = 11; /* 11bit */
	} else {
		asm_error(ctx, "Unknown immediate size for arch %u",
			  ctx->arch);
	}

	/* First create a mask with all possible bits for
	 * an immediate value unset. */
	mask = (~0 << immediate_size) & 0xFFFF;
	/* Is the sign bit of the immediate set? */
	if (imm & (1 << (immediate_size - 1))) {
		/* Yes, so all bits above that must also
		 * be set, otherwise we can't represent this
		 * value in an operand. */
		if ((imm & mask) != mask)
			return 0;
	} else {
		/* All bits above the immediate's size must
		 * be unset. */
		if (imm & mask)
			return 0;
	}

	return 1;
}

/* This checks if the value is nonzero and a power of two. */
static bool is_power_of_two(unsigned int value)
{
	return (value && ((value & (value - 1)) == 0));
}

/* This checks if all bits set in the mask are contiguous.
 * Zero is also considered a contiguous mask. */
static bool is_contiguous_bitmask(unsigned int mask)
{
	unsigned int low_zeros_mask;
	bool is_contiguous;

	if (mask == 0)
		return 1;
	/* Turn the lowest zeros of the mask into a bitmask.
	 * Example:  0b00011000 -> 0b00000111 */
	low_zeros_mask = (mask - 1) & ~mask;
	/* Adding the low_zeros_mask to the original mask
	 * basically is a bitwise OR operation.
	 * If the original mask was contiguous, we end up with a
	 * contiguous bitmask from bit 0 to the highest bit
	 * set in the original mask. Adding 1 will result in a single
	 * bit set, which is a power of two. */
	is_contiguous = is_power_of_two(mask + low_zeros_mask + 1);

	return is_contiguous;
}

static unsigned int generate_imm_operand(struct assembler_context *ctx,
					 const struct immediate *imm)
{
	unsigned int val, tmp;
	unsigned int mask;

	/* format: 0b11ii iiii iiii */

	val = 0xC00;
	if (ctx->arch == 15)
		val <<= 1;
	tmp = imm->imm;

	if (!is_valid_imm(ctx, tmp)) {
		asm_warn(ctx, "IMMEDIATE 0x%X (%d) too long "
			      "(> 9 bits + sign). Did you intend to "
			      "use implicit sign extension?",
			 tmp, (int)tmp);
	}

	if (ctx->arch == 15)
		tmp &= 0x7FF;
	else
		tmp &= 0x3FF;
	val |= tmp;

	return val;
}

static unsigned int generate_reg_operand(struct assembler_context *ctx,
					 const struct registr *reg)
{
	unsigned int val = 0;

	switch (reg->type) {
	case GPR:
		/* format: 0b1011 11rr rrrr */
		val |= 0xBC0;
		if (ctx->arch == 15)
			val <<= 1;
		if (reg->nr & ~0x3F) //FIXME 128 regs for v15 arch possible?
			asm_error(ctx, "GPR-nr too big");
		val |= reg->nr;
		break;
	case SPR:
		/* format: 0b100. .... .... */
		val |= 0x800;
		if (ctx->arch == 15)
			val <<= 1;
		if (reg->nr & ~0x1FF)
			asm_error(ctx, "SPR-nr too big");
		val |= reg->nr;
		break;
	case OFFR:
		/* format: 0b1000 0110 0rrr */
		val |= 0x860;
		if (ctx->arch == 15)
			val <<= 1;
		if (reg->nr & ~0x7)
			asm_error(ctx, "OFFR-nr too big");
		val |= reg->nr;
		break;
	default:
		asm_error(ctx, "generate_reg_operand() regtype");
	}

	return val;
}

static unsigned int generate_mem_operand(struct assembler_context *ctx,
					 const struct memory *mem)
{
	unsigned int val = 0, off, reg;

	switch (mem->type) {
	case MEM_DIRECT:
		/* format: 0b0mmm mmmm mmmm */
		off = mem->offset;
		if (off & ~0x7FF) { //FIXME 4096 words for v15 arch possible?
			asm_warn(ctx, "DIRECT memoffset 0x%X too long (> 11 bits)", off);
			off &= 0x7FF;
		}
		val |= off;
		break;
	case MEM_INDIRECT:
		/* format: 0b101r rroo oooo */
		off = mem->offset;
		reg = mem->offr_nr;
		val |= 0xA00;
		//FIXME what about v15 arch?
		if (off & ~0x3F) {
			asm_warn(ctx, "INDIRECT memoffset 0x%X too long (> 6 bits)", off);
			off &= 0x3F;
		}
		if (reg > 6) {
			/* Assembler bug. The parser shouldn't pass this value. */
			asm_error(ctx, "OFFR-nr too big");
		}
		if (reg == 6) {
			asm_warn(ctx, "Using offset register 6. This register is broken "
				 "on certain devices. Use off0 to off5 only.");
		}
		val |= off;
		val |= (reg << 6);
		break;
	default:
		asm_error(ctx, "generate_mem_operand() memtype");
	}

	return val;
}

static void generate_operand(struct assembler_context *ctx,
			     const struct operand *oper,
			     struct out_operand *out)
{
	out->type = OUTOPER_NORMAL;

	switch (oper->type) {
	case OPER_IMM:
		out->u.operand = generate_imm_operand(ctx, oper->u.imm);
		break;
	case OPER_REG:
		out->u.operand = generate_reg_operand(ctx, oper->u.reg);
		break;
	case OPER_MEM:
		out->u.operand = generate_mem_operand(ctx, oper->u.mem);
		break;
	case OPER_LABEL:
		out->type = OUTOPER_LABELREF;
		out->u.label = oper->u.label;
		break;
	case OPER_ADDR:
		out->u.operand = oper->u.addr->addr;
		break;
	case OPER_RAW:
		out->u.operand = oper->u.raw;
		break;
	default:
		asm_error(ctx, "generate_operand() operstate");
	}
}

static struct code_output * do_assemble_insn(struct assembler_context *ctx,
					     struct instruction *insn,
					     unsigned int opcode)
{
	int i;
	struct operlist *ol;
	int nr_oper = 0;
	uint64_t code = 0;
	struct code_output *out;
	struct label *labelref = NULL;
	struct operand *oper;
	int have_spr_operand = 0;
	int have_mem_operand = 0;

	out = xmalloc(sizeof(*out));
	INIT_LIST_HEAD(&out->list);
	out->opcode = opcode;

	ol = insn->operands;
	if (ARRAY_SIZE(out->operands) > ARRAY_SIZE(ol->oper))
		asm_error(ctx, "Internal operand array confusion");

	for (i = 0; i < ARRAY_SIZE(out->operands); i++) {
		oper = ol->oper[i];
		if (!oper)
			continue;

		/* If this is an INPUT operand (first or second), we must
		 * make sure that not both are accessing SPR or MEMORY.
		 * The device only supports one SPR or MEMORY operand in
		 * the input operands. */
		if ((i == 0) || (i == 1)) {
			if ((oper->type == OPER_REG) &&
			    (oper->u.reg->type == SPR)) {
				if (have_spr_operand)
					asm_error(ctx, "Multiple SPR input operands in one instruction");
				have_spr_operand = 1;
			}
			if (oper->type == OPER_MEM) {
				if (have_mem_operand)
					asm_error(ctx, "Multiple MEMORY input operands in on instruction");
				have_mem_operand = 1;
			}
		}

		generate_operand(ctx, oper, &out->operands[i]);
		nr_oper++;
	}
	if (nr_oper != 3)
		asm_error(ctx, "Internal error: nr_oper at "
			       "lowlevel do_assemble_insn");

	list_add_tail(&out->list, &ctx->output);

	return out;
}

static unsigned int merge_ext_into_opcode(struct assembler_context *ctx,
					  unsigned int opbase,
					  struct instruction *insn)
{
	struct operlist *ol;
	unsigned int opcode;
	unsigned int mask, shift;

	ol = insn->operands;
	opcode = opbase;
	mask = ol->oper[0]->u.raw;
	if (mask & ~0xF)
		asm_error(ctx, "opcode MASK extension too big (> 0xF)");
	shift = ol->oper[1]->u.raw;
	if (shift & ~0xF)
		asm_error(ctx, "opcode SHIFT extension too big (> 0xF)");
	opcode |= (mask << 4);
	opcode |= shift;
	ol->oper[0] = ol->oper[2];
	ol->oper[1] = ol->oper[3];
	ol->oper[2] = ol->oper[4];

	return opcode;
}

static unsigned int merge_external_jmp_into_opcode(struct assembler_context *ctx,
						   unsigned int opbase,
						   struct instruction *insn)
{
	struct operand *fake;
	struct registr *fake_reg;
	struct operand *target;
	struct operlist *ol;
	unsigned int cond;
	unsigned int opcode;

	ol = insn->operands;
	opcode = opbase;
	cond = ol->oper[0]->u.imm->imm;
	if (cond & ~0xFF)
		asm_error(ctx, "External jump condition value too big (> 0xFF)");
	opcode |= cond;
	target = ol->oper[1];
	memset(ol->oper, 0, sizeof(ol->oper));

	/* This instruction has two fake r0 operands
	 * at position 0 and 1. */
	fake = xmalloc(sizeof(*fake));
	fake_reg = xmalloc(sizeof(*fake_reg));
	fake->type = OPER_REG;
	fake->u.reg = fake_reg;
	fake_reg->type = GPR;
	fake_reg->nr = 0;

	ol->oper[0] = fake;
	ol->oper[1] = fake;
	ol->oper[2] = target;

	return opcode;
}

static void assemble_instruction(struct assembler_context *ctx,
				 struct instruction *insn);

static void emulate_mov_insn(struct assembler_context *ctx,
			     struct instruction *insn)
{
	struct instruction em_insn;
	struct operlist em_ol;
	struct operand em_op_shift;
	struct operand em_op_mask;
	struct operand em_op_x;
	struct operand em_op_y;
	struct immediate em_imm_x;
	struct immediate em_imm_y;

	struct operand *in, *out;
	unsigned int tmp;

	/* This is a pseudo-OP. We emulate it by OR or ORX */

	in = insn->operands->oper[0];
	out = insn->operands->oper[1];

	em_insn.op = OP_OR;
	em_ol.oper[0] = in;
	em_imm_x.imm = 0;
	em_op_x.type = OPER_IMM;
	em_op_x.u.imm = &em_imm_x;
	em_ol.oper[1] = &em_op_x;
	em_ol.oper[2] = out;

	if (in->type == OPER_IMM) {
		tmp = in->u.imm->imm;
		if (!is_possible_imm(tmp))
			asm_error(ctx, "MOV operand 0x%X > 16bit", tmp);
		if (!is_valid_imm(ctx, tmp)) {
			/* Immediate too big for plain OR */
			em_insn.op = OP_ORX;

			em_op_mask.type = OPER_RAW;
			em_op_mask.u.raw = 0x7;
			em_op_shift.type = OPER_RAW;
			em_op_shift.u.raw = 0x8;

			em_imm_x.imm = (tmp & 0xFF00) >> 8;
			em_op_x.type = OPER_IMM;
			em_op_x.u.imm = &em_imm_x;

			em_imm_y.imm = (tmp & 0x00FF);
			em_op_y.type = OPER_IMM;
			em_op_y.u.imm = &em_imm_y;

			em_ol.oper[0] = &em_op_mask;
			em_ol.oper[1] = &em_op_shift;
			em_ol.oper[2] = &em_op_x;
			em_ol.oper[3] = &em_op_y;
			em_ol.oper[4] = out;
		}
	}

	em_insn.operands = &em_ol;
	assemble_instruction(ctx, &em_insn); /* recurse */
}

static void emulate_jmp_insn(struct assembler_context *ctx,
			     struct instruction *insn)
{
	struct instruction em_insn;
	struct operlist em_ol;
	struct immediate em_condition;
	struct operand em_cond_op;

	/* This is a pseudo-OP. We emulate it with
	 * JEXT 0x7F, target */

	em_insn.op = OP_JEXT;
	em_condition.imm = 0x7F; /* Ext cond: Always true */
	em_cond_op.type = OPER_IMM;
	em_cond_op.u.imm = &em_condition;
	em_ol.oper[0] = &em_cond_op;
	em_ol.oper[1] = insn->operands->oper[0]; /* Target */
	em_insn.operands = &em_ol;

	assemble_instruction(ctx, &em_insn); /* recurse */
}

static void emulate_jand_insn(struct assembler_context *ctx,
			      struct instruction *insn,
			      int inverted)
{
	struct code_output *out;
	struct instruction em_insn;
	struct operlist em_ol;
	struct operand em_op_shift;
	struct operand em_op_mask;
	struct operand em_op_y;
	struct immediate em_imm;

	struct operand *oper0, *oper1, *oper2;
	struct operand *imm_oper = NULL;
	unsigned int tmp;
	int first_bit, last_bit;

	oper0 = insn->operands->oper[0];
	oper1 = insn->operands->oper[1];
	oper2 = insn->operands->oper[2];

	if (oper0->type == OPER_IMM)
		imm_oper = oper0;
	if (oper1->type == OPER_IMM)
		imm_oper = oper1;
	if (oper0->type == OPER_IMM && oper1->type == OPER_IMM)
		imm_oper = NULL;

	if (imm_oper) {
		/* We have a single immediate operand.
		 * Check if it's representable by a normal JAND insn.
		 */
		tmp = imm_oper->u.imm->imm;
		if (!is_valid_imm(ctx, tmp)) {
			/* Nope, this must be emulated by JZX/JNZX */
			if (!is_contiguous_bitmask(tmp)) {
				asm_error(ctx, "Long bitmask 0x%X is not contiguous",
					  tmp);
			}

			first_bit = ffs(tmp);
			last_bit = ffs(~(tmp >> (first_bit - 1))) - 1 + first_bit - 1;

			if (inverted)
				em_insn.op = OP_JZX;
			else
				em_insn.op = OP_JNZX;
			em_op_shift.type = OPER_RAW;
			em_op_shift.u.raw = first_bit - 1;
			em_op_mask.type = OPER_RAW;
			em_op_mask.u.raw = last_bit - first_bit;

			em_imm.imm = 0;
			em_op_y.type = OPER_IMM;
			em_op_y.u.imm = &em_imm;

			em_ol.oper[0] = &em_op_mask;
			em_ol.oper[1] = &em_op_shift;
			if (oper0->type != OPER_IMM)
				em_ol.oper[2] = oper0;
			else
				em_ol.oper[2] = oper1;
			em_ol.oper[3] = &em_op_y;
			em_ol.oper[4] = oper2;

			em_insn.operands = &em_ol;

			assemble_instruction(ctx, &em_insn); /* recurse */
			return;
		}
	}

	/* Do a normal JAND/JNAND instruction */
	if (inverted)
		out = do_assemble_insn(ctx, insn, 0x040 | 0x1);
	else
		out = do_assemble_insn(ctx, insn, 0x040);
	out->is_jump_insn = 1;
}

static void assemble_instruction(struct assembler_context *ctx,
				 struct instruction *insn)
{
	struct code_output *out;
	unsigned int opcode;

	switch (insn->op) {
	case OP_ADD:
		do_assemble_insn(ctx, insn, 0x1C0);
		break;
	case OP_ADDSC:
		do_assemble_insn(ctx, insn, 0x1C2);
		break;
	case OP_ADDC:
		do_assemble_insn(ctx, insn, 0x1C1);
		break;
	case OP_ADDSCC:
		do_assemble_insn(ctx, insn, 0x1C3);
		break;
	case OP_SUB:
		do_assemble_insn(ctx, insn, 0x1D0);
		break;
	case OP_SUBSC:
		do_assemble_insn(ctx, insn, 0x1D2);
		break;
	case OP_SUBC:
		do_assemble_insn(ctx, insn, 0x1D1);
		break;
	case OP_SUBSCC:
		do_assemble_insn(ctx, insn, 0x1D3);
		break;
	case OP_SRA:
		do_assemble_insn(ctx, insn, 0x130);
		break;
	case OP_OR:
		do_assemble_insn(ctx, insn, 0x160);
		break;
	case OP_AND:
		do_assemble_insn(ctx, insn, 0x140);
		break;
	case OP_XOR:
		do_assemble_insn(ctx, insn, 0x170);
		break;
	case OP_SR:
		do_assemble_insn(ctx, insn, 0x120);
		break;
	case OP_SRX:
		opcode = merge_ext_into_opcode(ctx, 0x200, insn);
		do_assemble_insn(ctx, insn, opcode);
		break;
	case OP_SL:
		do_assemble_insn(ctx, insn, 0x110);
		break;
	case OP_RL:
		do_assemble_insn(ctx, insn, 0x1A0);
		break;
	case OP_RR:
		do_assemble_insn(ctx, insn, 0x1B0);
		break;
	case OP_NAND:
		do_assemble_insn(ctx, insn, 0x150);
		break;
	case OP_ORX:
		opcode = merge_ext_into_opcode(ctx, 0x300, insn);
		do_assemble_insn(ctx, insn, opcode);
		break;
	case OP_MOV:
		emulate_mov_insn(ctx, insn);
		return;
	case OP_JMP:
		emulate_jmp_insn(ctx, insn);
		return;
	case OP_JAND:
		emulate_jand_insn(ctx, insn, 0);
		return;
	case OP_JNAND:
		emulate_jand_insn(ctx, insn, 1);
		return;
	case OP_JS:
		out = do_assemble_insn(ctx, insn, 0x050);
		out->is_jump_insn = 1;
		break;
	case OP_JNS:
		out = do_assemble_insn(ctx, insn, 0x050 | 0x1);
		out->is_jump_insn = 1;
		break;
	case OP_JE:
		out = do_assemble_insn(ctx, insn, 0x0D0);
		out->is_jump_insn = 1;
		break;
	case OP_JNE:
		out = do_assemble_insn(ctx, insn, 0x0D0 | 0x1);
		out->is_jump_insn = 1;
		break;
	case OP_JLS:
		out = do_assemble_insn(ctx, insn, 0x0D2);
		out->is_jump_insn = 1;
		break;
	case OP_JGES:
		out = do_assemble_insn(ctx, insn, 0x0D2 | 0x1);
		out->is_jump_insn = 1;
		break;
	case OP_JGS:
		out = do_assemble_insn(ctx, insn, 0x0D4);
		out->is_jump_insn = 1;
		break;
	case OP_JLES:
		out = do_assemble_insn(ctx, insn, 0x0D4 | 0x1);
		out->is_jump_insn = 1;
		break;
	case OP_JL:
		out = do_assemble_insn(ctx, insn, 0x0DA);
		out->is_jump_insn = 1;
		break;
	case OP_JGE:
		out = do_assemble_insn(ctx, insn, 0x0DA | 0x1);
		out->is_jump_insn = 1;
		break;
	case OP_JG:
		out = do_assemble_insn(ctx, insn, 0x0DC);
		break;
	case OP_JLE:
		out = do_assemble_insn(ctx, insn, 0x0DC | 0x1);
		out->is_jump_insn = 1;
		break;
	case OP_JZX:
		opcode = merge_ext_into_opcode(ctx, 0x400, insn);
		out = do_assemble_insn(ctx, insn, opcode);
		out->is_jump_insn = 1;
		break;
	case OP_JNZX:
		opcode = merge_ext_into_opcode(ctx, 0x500, insn);
		out = do_assemble_insn(ctx, insn, opcode);
		out->is_jump_insn = 1;
		break;
	case OP_JEXT:
		opcode = merge_external_jmp_into_opcode(ctx, 0x700, insn);
		out = do_assemble_insn(ctx, insn, opcode);
		out->is_jump_insn = 1;
		break;
	case OP_JNEXT:
		opcode = merge_external_jmp_into_opcode(ctx, 0x600, insn);
		out = do_assemble_insn(ctx, insn, opcode);
		out->is_jump_insn = 1;
		break;
	case OP_CALL:
		do_assemble_insn(ctx, insn, 0x002);
		break;
	case OP_RET:
		/* Get the previous instruction and check whether it
		 * is a jump instruction. */
		list_for_each_entry_reverse(out, &ctx->output, list) {
			/* Search the last insn. */
			if (out->type == OUT_INSN) {
				if (out->is_jump_insn) {
					asm_warn(ctx, "RET instruction directly after "
						 "jump instruction. The hardware won't like this.");
				}
				break;
			}
		}
		do_assemble_insn(ctx, insn, 0x003);
		break;
	case OP_TKIPH:
	case OP_TKIPHS:
	case OP_TKIPL:
	case OP_TKIPLS:
		do_assemble_insn(ctx, insn, 0x1E0);
		break;
	case OP_NAP:
		do_assemble_insn(ctx, insn, 0x001);
		break;
	case RAW_CODE:
		do_assemble_insn(ctx, insn, insn->opcode);
		break;
	default:
		asm_error(ctx, "Unknown op");
	}
}

static void assemble_instructions(struct assembler_context *ctx)
{
	struct statement *s;
	struct instruction *insn;
	struct code_output *out;

	if (ctx->start_label) {
		/* Generate a jump instruction at offset 0 to
		 * jump to the code start.
		 */
		struct instruction sjmp;
		struct operlist ol;
		struct operand oper;

		oper.type = OPER_LABEL;
		oper.u.label = ctx->start_label;
		ol.oper[0] = &oper;
		sjmp.op = OP_JMP;
		sjmp.operands = &ol;

		assemble_instruction(ctx, &sjmp);
		out = list_entry(ctx->output.next, struct code_output, list);
		out->is_start_insn = 1;
	}

	for_each_statement(ctx, s) {
		switch (s->type) {
		case STMT_INSN:
			ctx->cur_stmt = s;
			insn = s->u.insn;
			assemble_instruction(ctx, insn);
			break;
		case STMT_LABEL:
			out = xmalloc(sizeof(*out));
			INIT_LIST_HEAD(&out->list);
			out->type = OUT_LABEL;
			out->labelname = s->u.label->name;

			list_add_tail(&out->list, &ctx->output);
			break;
		case STMT_ASMDIR:
			break;
		}
	} for_each_statement_end(ctx, s);
}

/* Resolve a label reference to the address it points to. */
static int get_labeladdress(struct assembler_context *ctx,
			    struct code_output *this_insn,
			    struct label *labelref)
{
	struct code_output *c;
	bool found = 0;
	int address = -1;

	switch (labelref->direction) {
	case LABELREF_ABSOLUTE:
		list_for_each_entry(c, &ctx->output, list) {
			if (c->type != OUT_LABEL)
				continue;
			if (strcmp(c->labelname, labelref->name) != 0)
				continue;
			if (found) {
				asm_error(ctx, "Ambiguous label reference \"%s\"",
					  labelref->name);
			}
			found = 1;
			address = c->address;
		}
		break;
	case LABELREF_RELATIVE_BACK:
		for (c = list_entry(this_insn->list.prev, typeof(*c), list);
		     &c->list != &ctx->output;
		     c = list_entry(c->list.prev, typeof(*c), list)) {
			if (c->type != OUT_LABEL)
				continue;
			if (strcmp(c->labelname, labelref->name) == 0) {
				/* Found */
				address = c->address;
				break;
			}
		}
		break;
	case LABELREF_RELATIVE_FORWARD:
		for (c = list_entry(this_insn->list.next, typeof(*c), list);
		     &c->list != &ctx->output;
		     c = list_entry(c->list.next, typeof(*c), list)) {
			if (c->type != OUT_LABEL)
				continue;
			if (strcmp(c->labelname, labelref->name) == 0) {
				/* Found */
				address = c->address;
				break;
			}
		}
		break;
	}

	return address;
}

static void resolve_labels(struct assembler_context *ctx)
{
	struct code_output *c;
	int addr;
	int i;
	unsigned int current_address;

	/* Calculate the absolute addresses for each instruction. */
recalculate_addresses:
	current_address = 0;
	list_for_each_entry(c, &ctx->output, list) {
		switch (c->type) {
		case OUT_INSN:
			c->address = current_address;
			current_address++;
			break;
		case OUT_LABEL:
			c->address = current_address;
			break;
		}
	}

	/* Resolve the symbolic label references. */
	list_for_each_entry(c, &ctx->output, list) {
		switch (c->type) {
		case OUT_INSN:
			if (c->is_start_insn) {
				/* If the first %start-jump jumps to 001, we can
				 * optimize it away, as it's unneeded.
				 */
				i = 2;
				if (c->operands[i].type != OUTOPER_LABELREF)
					asm_error(ctx, "Internal error, %%start insn oper 2 not labelref");
				if (c->operands[i].u.label->direction != LABELREF_ABSOLUTE)
					asm_error(ctx, "%%start label reference not absolute");
				addr = get_labeladdress(ctx, c, c->operands[i].u.label);
				if (addr < 0)
					goto does_not_exist;
				if (addr == 1) {
					list_del(&c->list); /* Kill it */
					goto recalculate_addresses;
				}
			}

			for (i = 0; i < ARRAY_SIZE(c->operands); i++) {
				if (c->operands[i].type != OUTOPER_LABELREF)
					continue;
				addr = get_labeladdress(ctx, c, c->operands[i].u.label);
				if (addr < 0)
					goto does_not_exist;
				c->operands[i].u.operand = addr;
				if (i != 2) {
					/* Is not a jump target.
					 * Make it be an immediate */
					if (ctx->arch == 5)
						c->operands[i].u.operand |= 0xC00;
					else if (ctx->arch == 15)
						c->operands[i].u.operand |= 0xC00 << 1;
					else
						asm_error(ctx, "Internal error: label res imm");
				}
			}
			break;
		case OUT_LABEL:
			break;
		}
	}

	return;
does_not_exist:
	asm_error(ctx, "Label \"%s\" does not exist",
		  c->operands[i].u.label->name);
}

static void emit_code(struct assembler_context *ctx)
{
	FILE *fd;
	const char *fn;
	struct code_output *c;
	uint64_t code;
	unsigned char outbuf[8];
	unsigned int insn_count = 0;
	struct fw_header hdr;

	fn = outfile_name;
	fd = fopen(fn, "w+");
	if (!fd) {
		fprintf(stderr, "Could not open microcode output file \"%s\"\n", fn);
		exit(1);
	}
	if (IS_VERBOSE_DEBUG)
		fprintf(stderr, "\nCode:\n");

	list_for_each_entry(c, &ctx->output, list) {
		switch (c->type) {
		case OUT_INSN:
			insn_count++;
			break;
		default:
			break;
		}
	}

	memset(&hdr, 0, sizeof(hdr));
	hdr.type = FW_TYPE_UCODE;
	hdr.ver = FW_HDR_VER;
	hdr.size = cpu_to_be32(8 * insn_count);
	if (fwrite(&hdr, sizeof(hdr), 1, fd) != 1) {
		fprintf(stderr, "Could not write microcode outfile\n");
		exit(1);
	}

	if (insn_count > NUM_INSN_LIMIT)
		asm_warn(ctx, "Generating more than %d instructions. This "
			      "will overflow the device microcode memory.",
			 NUM_INSN_LIMIT);

	list_for_each_entry(c, &ctx->output, list) {
		switch (c->type) {
		case OUT_INSN:
			if (IS_VERBOSE_DEBUG) {
				fprintf(stderr, "%03X %03X,%03X,%03X\n",
					c->opcode,
					c->operands[0].u.operand,
					c->operands[1].u.operand,
					c->operands[2].u.operand);
			}
			code = 0;

			if (ctx->arch == 5) {
				/* Instruction binary format is: xxyyyzzz0000oooX
				 *                        byte-0-^       byte-7-^
				 * ooo is the opcode
				 * Xxx is the first operand
				 * yyy is the second operand
				 * zzz is the third operand
				 */
				code |= ((uint64_t)c->operands[2].u.operand);
				code |= ((uint64_t)c->operands[1].u.operand) << 12;
				code |= ((uint64_t)c->operands[0].u.operand) << 24;
				code |= ((uint64_t)c->opcode) << 36;
				code = ((code & (uint64_t)0xFFFFFFFF00000000ULL) >> 32) |
				       ((code & (uint64_t)0x00000000FFFFFFFFULL) << 32);
			} else if (ctx->arch == 15) {
				code |= ((uint64_t)c->operands[2].u.operand);
				code |= ((uint64_t)c->operands[1].u.operand) << 13;
				code |= ((uint64_t)c->operands[0].u.operand) << 26;
				code |= ((uint64_t)c->opcode) << 39;
				code = ((code & (uint64_t)0xFFFFFFFF00000000ULL) >> 32) |
				       ((code & (uint64_t)0x00000000FFFFFFFFULL) << 32);
			} else {
				asm_error(ctx, "No emit format for arch %u",
					  ctx->arch);
			}
			outbuf[0] = (code & (uint64_t)0xFF00000000000000ULL) >> 56;
			outbuf[1] = (code & (uint64_t)0x00FF000000000000ULL) >> 48;
			outbuf[2] = (code & (uint64_t)0x0000FF0000000000ULL) >> 40;
			outbuf[3] = (code & (uint64_t)0x000000FF00000000ULL) >> 32;
			outbuf[4] = (code & (uint64_t)0x00000000FF000000ULL) >> 24;
			outbuf[5] = (code & (uint64_t)0x0000000000FF0000ULL) >> 16;
			outbuf[6] = (code & (uint64_t)0x000000000000FF00ULL) >> 8;
			outbuf[7] = (code & (uint64_t)0x00000000000000FFULL) >> 0;

			if (fwrite(&outbuf, ARRAY_SIZE(outbuf), 1, fd) != 1) {
				fprintf(stderr, "Could not write microcode outfile\n");
				exit(1);
			}
			break;
		case OUT_LABEL:
			break;
		}
	}

	if (arg_print_sizes) {
		printf("%s:  text = %u instructions (%u bytes)\n",
		       fn, insn_count,
		       (unsigned int)(insn_count * sizeof(uint64_t)));
	}

	fclose(fd);
}

static void assemble(void)
{
	struct assembler_context ctx;

	memset(&ctx, 0, sizeof(ctx));
	INIT_LIST_HEAD(&ctx.output);

	eval_directives(&ctx);
	assemble_instructions(&ctx);
	resolve_labels(&ctx);
	emit_code(&ctx);
}

static void initialize(void)
{
	INIT_LIST_HEAD(&infile.sl);
	INIT_LIST_HEAD(&infile.ivals);
#ifdef YYDEBUG
	if (IS_INSANE_DEBUG)
		yydebug = 1;
	else
		yydebug = 0;
#endif /* YYDEBUG */
}

int main(int argc, char **argv)
{
	int err, res = 1;

	err = parse_args(argc, argv);
	if (err < 0)
		goto out;
	if (err > 0) {
		res = 0;
		goto out;
	}
	err = open_input_file();
	if (err)
		goto out;
	initialize();
	yyparse();
	assemble();
	assemble_initvals();
	close_input_file();
	res = 0;
out:
	/* Lazyman simply leaks all allocated memory. */
	return res;
}
