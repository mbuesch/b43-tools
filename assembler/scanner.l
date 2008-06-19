%{

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

#include "parser.h"
#include "main.h"

#include <stdio.h>

#undef min
#define min(x,y)	((x) < (y) ? (x) : (y))


static void interpret_cppinfo(const char *);
static void update_lineinfo(void);

static inline void log_current_line(void)
{
	size_t len;

	len = min(sizeof(cur_lineinfo.linecopy) - 1, strlen(yytext));
	strncpy(cur_lineinfo.linecopy, yytext, len);
	cur_lineinfo.linecopy[len] = '\0';
}

%}

IDENTIFIER	([a-zA-Z_][0-9a-zA-Z_]*)
WS		([ \t])
NEWLINE		((\r)|(\n)|(\r\n))

%%

^.*$					{ log_current_line(); REJECT; }
#\ [0-9]+\ \".*\"[ ]*[0-9]*{NEWLINE}	{ interpret_cppinfo(yytext); }


{WS}+			{ update_lineinfo(); /* whitespace */ }
{NEWLINE}		{ cur_lineinfo.lineno++; update_lineinfo(); }

^{WS}*"%"{WS}*arch	{ update_lineinfo(); return ASM_ARCH; }
^{WS}*"%"{WS}*start	{ update_lineinfo(); return ASM_START; }
"%"{WS}*assert		{ update_lineinfo(); return ASM_ASSERT; }

^{WS}*\.text{WS}*$			{ update_lineinfo(); return SECTION_TEXT; }
^{WS}*\.initvals/\({IDENTIFIER}\)	{ update_lineinfo(); return SECTION_IVALS; }

spr[0-9a-fA-F]{3,3}	{ update_lineinfo(); return SPR; }
r/([0-9]|([1-5][0-9])|(6[0-3]))	{ update_lineinfo(); return GPR; }
off/[0-6]		{ update_lineinfo(); return OFFR; }
lr/[0-3]		{ update_lineinfo(); return LR; }

,			{ update_lineinfo(); return COMMA; }
;			{ update_lineinfo(); return SEMICOLON; }
\[			{ update_lineinfo(); return BRACK_OPEN; }
\]			{ update_lineinfo(); return BRACK_CLOSE; }
\(			{ update_lineinfo(); return PAREN_OPEN; }
\)			{ update_lineinfo(); return PAREN_CLOSE; }

==			{ update_lineinfo(); return EQUAL; }
!=			{ update_lineinfo(); return NOT_EQUAL; }
\|\|			{ update_lineinfo(); return LOGICAL_OR; }
\&\&			{ update_lineinfo(); return LOGICAL_AND; }
\+			{ update_lineinfo(); return PLUS; }
\-			{ update_lineinfo(); return MINUS; }
\*			{ update_lineinfo(); return MULTIPLY; }
\/			{ update_lineinfo(); return DIVIDE; }
\|			{ update_lineinfo(); return BITW_OR; }
\&			{ update_lineinfo(); return BITW_AND; }
\^			{ update_lineinfo(); return BITW_XOR; }
\~			{ update_lineinfo(); return BITW_NOT; }
\<\<			{ update_lineinfo(); return LEFTSHIFT; }
\>\>			{ update_lineinfo(); return RIGHTSHIFT; }

add			{ update_lineinfo(); return OP_ADD; }
add\.			{ update_lineinfo(); return OP_ADDSC; }
addc			{ update_lineinfo(); return OP_ADDC; }
addc\.			{ update_lineinfo(); return OP_ADDSCC; }

sub			{ update_lineinfo(); return OP_SUB; }
sub\.			{ update_lineinfo(); return OP_SUBSC; }
subc			{ update_lineinfo(); return OP_SUBC; }
subc\.			{ update_lineinfo(); return OP_SUBSCC; }

sra			{ update_lineinfo(); return OP_SRA; }
or			{ update_lineinfo(); return OP_OR; }
and			{ update_lineinfo(); return OP_AND; }
xor			{ update_lineinfo(); return OP_XOR; }
sr			{ update_lineinfo(); return OP_SR; }
srx			{ update_lineinfo(); return OP_SRX; }
sl			{ update_lineinfo(); return OP_SL; }
rl			{ update_lineinfo(); return OP_RL; }
rr			{ update_lineinfo(); return OP_RR; }
nand			{ update_lineinfo(); return OP_NAND; }
orx			{ update_lineinfo(); return OP_ORX; }
mov			{ update_lineinfo(); return OP_MOV; }

jmp			{ update_lineinfo(); return OP_JMP; }
jand			{ update_lineinfo(); return OP_JAND; }
jnand			{ update_lineinfo(); return OP_JNAND; }
js			{ update_lineinfo(); return OP_JS; }
jns			{ update_lineinfo(); return OP_JNS; }
je			{ update_lineinfo(); return OP_JE; }
jne			{ update_lineinfo(); return OP_JNE; }
jls			{ update_lineinfo(); return OP_JLS; }
jges			{ update_lineinfo(); return OP_JGES; }
jgs			{ update_lineinfo(); return OP_JGS; }
jles			{ update_lineinfo(); return OP_JLES; }
jl			{ update_lineinfo(); return OP_JL; }
jge			{ update_lineinfo(); return OP_JGE; }
jg			{ update_lineinfo(); return OP_JG; }
jle			{ update_lineinfo(); return OP_JLE; }
jzx			{ update_lineinfo(); return OP_JZX; }
jnzx			{ update_lineinfo(); return OP_JNZX; }
jext			{ update_lineinfo(); return OP_JEXT; }
jnext			{ update_lineinfo(); return OP_JNEXT; }

call			{ update_lineinfo(); return OP_CALL; }
ret			{ update_lineinfo(); return OP_RET; }

tkiph			{ update_lineinfo(); return OP_TKIPH; }
tkiphs			{ update_lineinfo(); return OP_TKIPHS; }
tkipl			{ update_lineinfo(); return OP_TKIPL; }
tkipls			{ update_lineinfo(); return OP_TKIPLS; }

nap			{ update_lineinfo(); return OP_NAP; }

mmio16			{ update_lineinfo(); return IVAL_MMIO16; }
mmio32			{ update_lineinfo(); return IVAL_MMIO32; }
phy			{ update_lineinfo(); return IVAL_PHY; }
radio			{ update_lineinfo(); return IVAL_RADIO; }
shm16			{ update_lineinfo(); return IVAL_SHM16; }
shm32			{ update_lineinfo(); return IVAL_SHM32; }
tram			{ update_lineinfo(); return IVAL_TRAM; }

@[0-9a-fA-F]{3,3}	{ update_lineinfo(); return RAW_CODE; }

0x[0-9a-fA-F]+		{ update_lineinfo(); return HEXNUM; }
-?[0-9]+		{ update_lineinfo(); return DECNUM; }

{IDENTIFIER}:		{ update_lineinfo(); return LABEL; }
{IDENTIFIER}		{ update_lineinfo(); return IDENT; }

%%

struct lineinfo cur_lineinfo;
//FIXME The linenumber sometimes is wrong.

static void interpret_cppinfo(const char *str)
{
	const char * const orig = str;
	char tmp[64];
	char *found;

	/* This will interpret lines added by CPP.
	 * They look like:
	 *     # 3 "file.asm" 1
	 */

	if (*str == '\0')
		goto error;
	str++; /* skip # character */
	if (*str == '\0')
		goto error;
	str++; /* skip whitespace */

	/* Line number */
	found = strchr(str, ' ');
	if (!found)
		goto error;
	memset(tmp, 0, sizeof(tmp));
	memcpy(tmp, str, min(sizeof(tmp) - 1,
			     (int)(found - str)));
	cur_lineinfo.lineno = strtoul(tmp, NULL, 10) - 1;
	str = found;
	str++;

	/* File name */
	if (*str != '\"')
		goto error;
	str++;
	if (*str == '\0')
		goto error;
	found = strchr(str, '\"');
	if (!found)
		goto error;
	memset(cur_lineinfo.file, 0, sizeof(cur_lineinfo.file));
	memcpy(cur_lineinfo.file, str,
	       min(sizeof(cur_lineinfo.file) - 1,
		   (int)(found - str)));

	return;
error:
	fprintf(stderr, "Invalid CPP line directive:  %s\n", orig);
	exit(1);
}

static void update_lineinfo(void)
{
	int i = 0;

	while (yytext[i] != '\0') {
		switch (yytext[i]) {
		case '\r':
		case '\n':
			cur_lineinfo.column = 0;
			break;
		case '\t':
			cur_lineinfo.column += 8 - (cur_lineinfo.column % 8);
			break;
		default:
			cur_lineinfo.column++;
		}
		i++;
	}
}
