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

#include "args.h"
#include "main.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


int _debug;

struct cmdline_args cmdargs = {
	.arch = 5,		/* Default to v5 architecture. */
	.no_header = 0,		/* Input file does not have a header. */
	.print_addresses = 0,	/* Print the code addresses in the output. */
};

#define ARG_MATCH		0
#define ARG_NOMATCH		1
#define ARG_ERROR		-1

static int do_cmp_arg(char **argv, int *pos,
		      const char *template,
		      int allow_merged,
		      char **param)
{
	char *arg;
	char *next_arg;
	size_t arg_len, template_len;

	arg = argv[*pos];
	next_arg = argv[*pos + 1];
	arg_len = strlen(arg);
	template_len = strlen(template);

	if (param) {
		/* Maybe we have a merged parameter here.
		 * A merged parameter is "-pfoobar" for example.
		 */
		if (allow_merged && arg_len > template_len) {
			if (memcmp(arg, template, template_len) == 0) {
				*param = arg + template_len;
				return ARG_MATCH;
			}
			return ARG_NOMATCH;
		} else if (arg_len != template_len)
			return ARG_NOMATCH;
		*param = next_arg;
	}
	if (strcmp(arg, template) == 0) {
		if (param) {
			/* Skip the parameter on the next iteration. */
			(*pos)++;
			if (*param == 0) {
				fprintf(stderr, "%s needs a parameter\n", arg);
				return ARG_ERROR;
			}
		}
		return ARG_MATCH;
	}

	return ARG_NOMATCH;
}

/* Simple and lean command line argument parsing. */
static int cmp_arg(char **argv, int *pos,
		   const char *long_template,
		   const char *short_template,
		   char **param)
{
	int err;

	if (long_template) {
		err = do_cmp_arg(argv, pos, long_template, 0, param);
		if (err == ARG_MATCH || err == ARG_ERROR)
			return err;
	}
	err = ARG_NOMATCH;
	if (short_template)
		err = do_cmp_arg(argv, pos, short_template, 1, param);
	return err;
}

static void usage(int argc, char **argv)
{
	fprintf(stderr, "Usage: %s INPUT_FILE OUTPUT_FILE [OPTIONS]\n", argv[0]);
	fprintf(stderr, "  -a|--arch ARCH      The architecture type of the input file\n");
	fprintf(stderr, "  -h|--help           Print this help\n");
	fprintf(stderr, "  --nohdr             The input file does not have a header\n");
	fprintf(stderr, "  --paddr             Print the code addresses\n");
	fprintf(stderr, "  -d|--debug          Print verbose debugging info\n");
	fprintf(stderr, "                      Repeat for more verbose debugging\n");
}

int parse_args(int argc, char **argv)
{
	int i;
	int res;
	char *param;

	infile_name = NULL;
	outfile_name = NULL;

	for (i = 1; i < argc; i++) {
		if ((res = cmp_arg(argv, &i, "--help", "-h", 0)) == ARG_MATCH) {
			usage(argc, argv);
			return 1;
		} else if ((res = cmp_arg(argv, &i, "--nohdr", 0, 0)) == ARG_MATCH) {
			cmdargs.no_header = 1;
		} else if ((res = cmp_arg(argv, &i, "--paddr", 0, 0)) == ARG_MATCH) {
			cmdargs.print_addresses = 1;
		} else if ((res = cmp_arg(argv, &i, "--debug", "-d", 0)) == ARG_MATCH) {
			_debug++;
		} else if ((res = cmp_arg(argv, &i, "--arch", "-a", &param)) == ARG_MATCH) {
			unsigned long arch;
			char *tail;

			arch = strtol(param, &tail, 0);
			if (strlen(tail) || (arch != 5)) {
				fprintf(stderr, "Unsupported architecture \"%s\"\n",
					param);
				return -1;
			}
			cmdargs.arch = arch;
		} else {
			if (!infile_name) {
				infile_name = argv[i];
				continue;
			}
			if (!outfile_name) {
				outfile_name = argv[i];
				continue;
			}
			fprintf(stderr, "Unrecognized argument: %s\n", argv[i]);
			goto out_usage;
		}
	}
	if (!infile_name || !outfile_name)
		goto out_usage;

	return 0;
out_usage:
	usage(argc, argv);
	return -1;
}

int open_input_file(void)
{
	if (strcmp(infile_name, "-") == 0) {
		infile = stdin;
	} else {
		infile = fopen(infile_name, "r");
		if (!infile) {
			fprintf(stderr, "Could not open INPUT_FILE %s\n",
				infile_name);
			return -1;
		}
	}

	return 0;
}

void close_input_file(void)
{
	if (strcmp(infile_name, "-") != 0)
		fclose(infile);
}

int open_output_file(void)
{
	if (strcmp(outfile_name, "-") == 0) {
		outfile = stdout;
	} else {
		outfile = fopen(outfile_name, "w+");
		if (!outfile) {
			fprintf(stderr, "Could not open OUTPUT_FILE %s\n",
				outfile_name);
			return -1;
		}
	}

	return 0;
}

void close_output_file(void)
{
	if (strcmp(outfile_name, "-") != 0)
		fclose(outfile);
}
