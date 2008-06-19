/*
 *   Copyright (C) 2006  Michael Buesch <mb@bu3sch.de>
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
#include "args.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


struct bin_instruction {
	unsigned int opcode;
	unsigned int operands[3];
};

struct statement {
	enum {
		STMT_INSN,
		STMT_LABEL,
	} type;

	union {
		struct {
			struct bin_instruction *bin;
			const char *name;
			const char *operands[5];

			int is_labelref;
			unsigned int labeladdr;
			struct statement *labelref;
		} insn;
		struct {
			char *name;
		} label;
	} u;

	struct list_head list;
};

struct disassembler_context {
	/* The architecture of the input file. */
	unsigned int arch;

	struct bin_instruction *code;
	size_t nr_insns;

	struct list_head stmt_list;
};


FILE *infile;
FILE *outfile;
const char *infile_name;
const char *outfile_name;


static const char * gen_raw_code(unsigned int operand)
{
	char *ret;

	ret = xmalloc(5);
	snprintf(ret, 5, "@%03X", operand);

	return ret;
}

static const char * disasm_mem_operand(unsigned int operand)
{
	char *ret;

	ret = xmalloc(8);
	snprintf(ret, 8, "[0x%03X]", operand);

	return ret;
}

static const char * disasm_indirect_mem_operand(unsigned int operand)
{
	char *ret;

	ret = xmalloc(12);
	snprintf(ret, 12, "[0x%02X,off%u]",
		 (operand & 0x3F), ((operand >> 6) & 0x7));

	return ret;
}

static const char * disasm_imm_operand(unsigned int operand)
{
	char *ret;

	operand &= ~0xC00;

	ret = xmalloc(7);
	if (operand & (1 << 9))
		snprintf(ret, 7, "0x%04X", (operand | 0xFC00));
	else
		snprintf(ret, 7, "0x%03X", operand);

	return ret;
}

static const char * disasm_spr_operand(unsigned int operand)
{
	char *ret;

	ret = xmalloc(7);
	snprintf(ret, 7, "spr%03X", (operand & 0x1FF));

	return ret;
}

static const char * disasm_gpr_operand(unsigned int operand)
{
	char *ret;

	ret = xmalloc(4);
	snprintf(ret, 4, "r%u", (operand & 0x3F));

	return ret;
}

static const char * disasm_offr_operand(unsigned int operand)
{
	char *ret;

	ret = xmalloc(5);
	snprintf(ret, 5, "off%u", (operand & 0x7));

	return ret;
}

static void disasm_std_operand(struct statement *stmt,
			       int oper_idx,
			       int out_idx,
			       int forceraw)
{
	unsigned int operand = stmt->u.insn.bin->operands[oper_idx];

	if (forceraw)
		goto raw;

	if (!(operand & 0x800)) {
		stmt->u.insn.operands[out_idx] = disasm_mem_operand(operand);
		return;
	} else if ((operand & 0xC00) == 0xC00) { 
		stmt->u.insn.operands[out_idx] = disasm_imm_operand(operand);
		return;
	} else if ((operand & 0xFC0) == 0xBC0) {
		stmt->u.insn.operands[out_idx] = disasm_gpr_operand(operand);
		return;
	} else if ((operand & 0xE00) == 0x800) {
		stmt->u.insn.operands[out_idx] = disasm_spr_operand(operand);
		return;
	} else if ((operand & 0xFF8) == 0x860) {
		stmt->u.insn.operands[out_idx] = disasm_offr_operand(operand);
		return;
	} else if ((operand & 0xE00) == 0xA00) {
		stmt->u.insn.operands[out_idx] = disasm_indirect_mem_operand(operand);
		return;
	}
raw:
	stmt->u.insn.operands[out_idx] = gen_raw_code(operand);
}

static void disasm_constant_opcodes(struct disassembler_context *ctx,
				    struct statement *stmt)
{
	struct bin_instruction *bin = stmt->u.insn.bin;

	switch (bin->opcode) {
	case 0x1C0:
		stmt->u.insn.name = "add";
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	case 0x1C2:
		stmt->u.insn.name = "add.";
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	case 0x1C1:
		stmt->u.insn.name = "addc";
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	case 0x1C3:
		stmt->u.insn.name = "addc.";
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	case 0x1D0:
		stmt->u.insn.name = "sub";
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	case 0x1D2:
		stmt->u.insn.name = "sub.";
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	case 0x1D1:
		stmt->u.insn.name = "subc";
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	case 0x1D3:
		stmt->u.insn.name = "subc.";
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	case 0x130:
		stmt->u.insn.name = "sra";
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	case 0x160:
		stmt->u.insn.name = "or";
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	case 0x140:
		stmt->u.insn.name = "and";
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	case 0x170:
		stmt->u.insn.name = "xor";
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	case 0x120:
		stmt->u.insn.name = "sr";
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	case 0x110:
		stmt->u.insn.name = "sl";
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	case 0x1A0:
		stmt->u.insn.name = "rl";
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	case 0x1B0:
		stmt->u.insn.name = "rr";
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	case 0x150:
		stmt->u.insn.name = "nand";
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	case 0x040:
		stmt->u.insn.name = "jand";
		stmt->u.insn.is_labelref = 2;
		stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		break;
	case (0x040 | 0x1):
		stmt->u.insn.name = "jnand";
		stmt->u.insn.is_labelref = 2;
		stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		break;
	case 0x050:
		stmt->u.insn.name = "js";
		stmt->u.insn.is_labelref = 2;
		stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		break;
	case (0x050 | 0x1):
		stmt->u.insn.name = "jns";
		stmt->u.insn.is_labelref = 2;
		stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		break;
	case 0x0D0:
		stmt->u.insn.name = "je";
		stmt->u.insn.is_labelref = 2;
		stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		break;
	case (0x0D0 | 0x1):
		stmt->u.insn.name = "jne";
		stmt->u.insn.is_labelref = 2;
		stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		break;
	case 0x0D2:
		stmt->u.insn.name = "jls";
		stmt->u.insn.is_labelref = 2;
		stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		break;
	case (0x0D2 | 0x1):
		stmt->u.insn.name = "jges";
		stmt->u.insn.is_labelref = 2;
		stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		break;
	case 0x0D4:
		stmt->u.insn.name = "jgs";
		stmt->u.insn.is_labelref = 2;
		stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		break;
	case (0x0D4 | 0x1):
		stmt->u.insn.name = "jles";
		stmt->u.insn.is_labelref = 2;
		stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		break;
	case 0x0DA:
		stmt->u.insn.name = "jl";
		stmt->u.insn.is_labelref = 2;
		stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		break;
	case (0x0DA | 0x1):
		stmt->u.insn.name = "jge";
		stmt->u.insn.is_labelref = 2;
		stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		break;
	case 0x0DC:
		stmt->u.insn.name = "jg";
		stmt->u.insn.is_labelref = 2;
		stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		break;
	case (0x0DC | 0x1):
		stmt->u.insn.name = "jle";
		stmt->u.insn.is_labelref = 2;
		stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 1, 1, 0);
		break;
	case 0x002: {
		char *str;

		stmt->u.insn.name = "call";
		stmt->u.insn.is_labelref = 1;
		stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
		str = xmalloc(4);
		snprintf(str, 4, "lr%u", stmt->u.insn.bin->operands[0]);
		stmt->u.insn.operands[0] = str;
		break;
	}
	case 0x003: {
		char *str;

		stmt->u.insn.name = "ret";
		str = xmalloc(4);
		snprintf(str, 4, "lr%u", stmt->u.insn.bin->operands[0]);
		stmt->u.insn.operands[0] = str;
		str = xmalloc(4);
		snprintf(str, 4, "lr%u", stmt->u.insn.bin->operands[2]);
		stmt->u.insn.operands[2] = str;
		break;
	}
	case 0x1E0: {
		unsigned int flags;

		flags = stmt->u.insn.bin->operands[1];
		switch (flags & ~0xC00) {
		case 0x1:
			stmt->u.insn.name = "tkiph";
			break;
		case (0x1 | 0x2):
			stmt->u.insn.name = "tkiphs";
			break;
		case 0x0:
			stmt->u.insn.name = "tkipl";
			break;
		case (0x0 | 0x2):
			stmt->u.insn.name = "tkipls";
			break;
		default:
			fprintf(stderr, "Invalid TKIP flags %X\n",
				flags);
			exit(1);
		}
		disasm_std_operand(stmt, 0, 0, 0);
		disasm_std_operand(stmt, 2, 2, 0);
		break;
	}
	case 0x001: {
		stmt->u.insn.name = "nap";
		if (stmt->u.insn.bin->operands[0] != 0xBC0) {
			fprintf(stderr, "NAP: invalid first argument 0x%03X\n",
				stmt->u.insn.bin->operands[0]);
		}
		if (stmt->u.insn.bin->operands[1] != 0xBC0) {
			fprintf(stderr, "NAP: invalid second argument 0x%03X\n",
				stmt->u.insn.bin->operands[1]);
		}
		if (stmt->u.insn.bin->operands[2] != 0x000) {
			fprintf(stderr, "NAP: invalid third argument 0x%03X\n",
				stmt->u.insn.bin->operands[2]);
		}
		break;
	}
	default:
		stmt->u.insn.name = gen_raw_code(bin->opcode);
		disasm_std_operand(stmt, 0, 0, 1);
		disasm_std_operand(stmt, 1, 1, 1);
		disasm_std_operand(stmt, 2, 2, 1);
		break;
	}
}

static void disasm_opcodes(struct disassembler_context *ctx)
{
	struct bin_instruction *bin;
	size_t i;
	struct statement *stmt;
	char *str;

	for (i = 0; i < ctx->nr_insns; i++) {
		bin = &(ctx->code[i]);

		stmt = xmalloc(sizeof(struct statement));
		stmt->type = STMT_INSN;
		INIT_LIST_HEAD(&stmt->list);
		stmt->u.insn.bin = bin;
		stmt->u.insn.is_labelref = -1;

		switch (bin->opcode & 0xF00) {
		case 0x200:
			stmt->u.insn.name = "srx";

			str = xmalloc(3);
			snprintf(str, 3, "%d", (bin->opcode & 0x0F0) >> 4);
			stmt->u.insn.operands[0] = str;
			str = xmalloc(3);
			snprintf(str, 3, "%d", (bin->opcode & 0x00F));
			stmt->u.insn.operands[1] = str;

			disasm_std_operand(stmt, 0, 2, 0);
			disasm_std_operand(stmt, 1, 3, 0);
			disasm_std_operand(stmt, 2, 4, 0);
			break;
		case 0x300:
			stmt->u.insn.name = "orx";

			str = xmalloc(3);
			snprintf(str, 3, "%d", (bin->opcode & 0x0F0) >> 4);
			stmt->u.insn.operands[0] = str;
			str = xmalloc(3);
			snprintf(str, 3, "%d", (bin->opcode & 0x00F));
			stmt->u.insn.operands[1] = str;

			disasm_std_operand(stmt, 0, 2, 0);
			disasm_std_operand(stmt, 1, 3, 0);
			disasm_std_operand(stmt, 2, 4, 0);
			break;
		case 0x400:
			stmt->u.insn.name = "jzx";

			str = xmalloc(3);
			snprintf(str, 3, "%d", (bin->opcode & 0x0F0) >> 4);
			stmt->u.insn.operands[0] = str;
			str = xmalloc(3);
			snprintf(str, 3, "%d", (bin->opcode & 0x00F));
			stmt->u.insn.operands[1] = str;

			disasm_std_operand(stmt, 0, 2, 0);
			disasm_std_operand(stmt, 1, 3, 0);
			stmt->u.insn.is_labelref = 4;
			stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
			break;
		case 0x500:
			stmt->u.insn.name = "jnzx";

			str = xmalloc(3);
			snprintf(str, 3, "%d", (bin->opcode & 0x0F0) >> 4);
			stmt->u.insn.operands[0] = str;
			str = xmalloc(3);
			snprintf(str, 3, "%d", (bin->opcode & 0x00F));
			stmt->u.insn.operands[1] = str;

			disasm_std_operand(stmt, 0, 2, 0);
			disasm_std_operand(stmt, 1, 3, 0);
			stmt->u.insn.is_labelref = 4;
			stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
			break;
		case 0x600:
			stmt->u.insn.name = "jnext";

			str = xmalloc(5);
			snprintf(str, 5, "0x%02X", (bin->opcode & 0x0FF));
			stmt->u.insn.operands[0] = str;

			/* We don't disassemble the first and second operand, as
			 * that always is a dummy r0 operand.
			 * disasm_std_operand(stmt, 0, 1, 0);
			 * disasm_std_operand(stmt, 1, 2, 0);
			 * stmt->u.insn.is_labelref = 3;
			 */
			stmt->u.insn.is_labelref = 1;
			stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
			break;
		case 0x700:
			stmt->u.insn.name = "jext";

			str = xmalloc(5);
			snprintf(str, 5, "0x%02X", (bin->opcode & 0x0FF));
			stmt->u.insn.operands[0] = str;

			/* We don't disassemble the first and second operand, as
			 * that always is a dummy r0 operand.
			 * disasm_std_operand(stmt, 0, 1, 0);
			 * disasm_std_operand(stmt, 1, 2, 0);
			 * stmt->u.insn.is_labelref = 3;
			 */
			stmt->u.insn.is_labelref = 1;
			stmt->u.insn.labeladdr = stmt->u.insn.bin->operands[2];
			break;
		default:
			disasm_constant_opcodes(ctx, stmt);
			break;
		}

		list_add_tail(&stmt->list, &ctx->stmt_list);
	}
}

static struct statement * get_label_at(struct disassembler_context *ctx,
				       unsigned int addr)
{
	unsigned int addrcnt = 0;
	struct statement *stmt, *ret, *prev;

	list_for_each_entry(stmt, &ctx->stmt_list, list) {
		if (stmt->type != STMT_INSN)
			continue;
		if (addrcnt == addr) {
			prev = list_entry(stmt->list.prev, struct statement, list);
			if (prev->type == STMT_LABEL)
				return prev;
			ret = xmalloc(sizeof(struct statement));
			INIT_LIST_HEAD(&ret->list);
			ret->type = STMT_LABEL;
			list_add(&ret->list, &prev->list);

			return ret;
		}
		addrcnt++;
	}

	return NULL;
}

static void resolve_labels(struct disassembler_context *ctx)
{
	struct statement *stmt;
	struct statement *label;
	struct statement *n;
	unsigned int labeladdr;
	unsigned int namecnt = 0;

	/* Resolve label references */
	list_for_each_entry_safe(stmt, n, &ctx->stmt_list, list) {
		if (stmt->type != STMT_INSN)
			continue;
		if (stmt->u.insn.is_labelref == -1)
			continue;
		labeladdr = stmt->u.insn.labeladdr;
		label = get_label_at(ctx, labeladdr);
		if (!label) {
			fprintf(stderr, "Labeladdress %X out of bounds\n",
				labeladdr);
			exit(1);
		}
		stmt->u.insn.labelref = label;
	}

	/* Name the labels */
	list_for_each_entry(stmt, &ctx->stmt_list, list) {
		if (stmt->type != STMT_LABEL)
			continue;
		stmt->u.label.name = xmalloc(20);
		snprintf(stmt->u.label.name, 20, "L%u", namecnt);
		namecnt++;
	}
}

static void emit_asm(struct disassembler_context *ctx)
{
	struct statement *stmt;
	int first, i;
	int err;
	unsigned int addr = 0;

	err = open_output_file();
	if (err)
		exit(1);

	fprintf(outfile, "%%arch %u\n\n", ctx->arch);
	list_for_each_entry(stmt, &ctx->stmt_list, list) {
		switch (stmt->type) {
		case STMT_INSN:
			if (cmdargs.print_addresses)
				fprintf(outfile, "/* %03X */", addr);
			fprintf(outfile, "\t%s", stmt->u.insn.name);
			first = 1;
			for (i = 0; i < ARRAY_SIZE(stmt->u.insn.operands); i++) {
				if (stmt->u.insn.is_labelref == i) {
					fprintf(outfile, ", %s",
						stmt->u.insn.labelref->u.label.name);
				}
				if (!stmt->u.insn.operands[i])
					continue;
				if (first)
					fprintf(outfile, "\t");
				if (!first)
					fprintf(outfile, ", ");
				first = 0;
				fprintf(outfile, "%s",
					stmt->u.insn.operands[i]);
			}
			fprintf(outfile, "\n");
			addr++;
			break;
		case STMT_LABEL:
			fprintf(outfile, "%s:\n", stmt->u.label.name);
			break;
		}
	}

	close_output_file();
}

static int read_input(struct disassembler_context *ctx)
{
	size_t size = 0, pos = 0;
	size_t ret;
	struct bin_instruction *code = NULL;
	unsigned char tmp[sizeof(uint64_t)];
	uint64_t codeword;
	struct fw_header hdr;
	int err;

	err = open_input_file();
	if (err)
		goto error;

	if (!cmdargs.no_header) {
		ret = fread(&hdr, 1, sizeof(hdr), infile);
		if (ret != sizeof(hdr)) {
			fprintf(stderr, "Corrupt input file (not fwcutter output)\n");
			goto err_close;
		}
		if (hdr.type != FW_TYPE_UCODE) {
			fprintf(stderr, "Corrupt input file. Not a microcode image.\n");
			goto err_close;
		}
		if (hdr.ver != FW_HDR_VER) {
			fprintf(stderr, "Invalid input file header version.\n");
			goto err_close;
		}
	}

	while (1) {
		if (pos >= size) {
			size += 512;
			code = xrealloc(code, size * sizeof(struct bin_instruction));
		}
		ret = fread(tmp, 1, sizeof(uint64_t), infile);
		if (!ret)
			break;
		if (ret != sizeof(uint64_t)) {
			fprintf(stderr, "Corrupt input file (not 8 byte aligned)\n");
			goto err_free_code;
		}

		codeword = 0;
		codeword |= ((uint64_t)tmp[0]) << 56;
		codeword |= ((uint64_t)tmp[1]) << 48;
		codeword |= ((uint64_t)tmp[2]) << 40;
		codeword |= ((uint64_t)tmp[3]) << 32;
		codeword |= ((uint64_t)tmp[4]) << 24;
		codeword |= ((uint64_t)tmp[5]) << 16;
		codeword |= ((uint64_t)tmp[6]) << 8;
		codeword |= ((uint64_t)tmp[7]);

		code[pos].opcode = (codeword >> 4) & 0xFFF;
		code[pos].operands[0] = (codeword & 0xF) << 8;
		code[pos].operands[0] |= (codeword >> 56) & 0xFF;
		code[pos].operands[1] = (codeword >> 44) & 0xFFF;
		code[pos].operands[2] = (codeword >> 32) & 0xFFF;

		pos++;
	}

	ctx->code = code;
	ctx->nr_insns = pos;

	close_input_file();

	return 0;

err_free_code:
	free(code);
err_close:
	close_input_file();
error:
	return -1;
}

static void disassemble(void)
{
	struct disassembler_context ctx;
	int err;

	memset(&ctx, 0, sizeof(ctx));
	INIT_LIST_HEAD(&ctx.stmt_list);
	ctx.arch = cmdargs.arch;

	err = read_input(&ctx);
	if (err)
		exit(1);
	disasm_opcodes(&ctx);
	resolve_labels(&ctx);
	emit_asm(&ctx);
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
	disassemble();
	res = 0;
out:
	/* Lazyman simply leaks all allocated memory. */
	return res;
}
