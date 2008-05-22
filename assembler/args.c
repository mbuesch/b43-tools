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
bool arg_print_sizes;
const char *initvals_fn_extension = ".initvals";
const char *real_infile_name;


#define ARG_MATCH		0
#define ARG_NOMATCH		1
#define ARG_ERROR		-1

static int do_cmp_arg(char **argv, int *pos,
		      const char *template,
		      int allow_merged,
		      const char **param)
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
		   const char **param)
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
	printf("Usage: %s INPUT_FILE OUTPUT_FILE [OPTIONS]\n", argv[0]);
	printf("  -h|--help           Print this help\n");
	printf("  -d|--debug          Print verbose debugging info\n");
	printf("                      Repeat for more verbose debugging\n");
	printf("  -s|--psize          Print the size of the code after assembling\n");
	printf("  -e|--ivalext EXT    Filename extension for the initvals\n");
}

int parse_args(int argc, char **argv)
{
	int i;
	int res;

	if (argc < 3)
		goto out_usage;
	infile_name = argv[1];
	outfile_name = argv[2];

	for (i = 3; i < argc; i++) {
		if ((res = cmp_arg(argv, &i, "--help", "-h", 0)) == ARG_MATCH) {
			usage(argc, argv);
			return 1;
		} else if ((res = cmp_arg(argv, &i, "--debug", "-d", 0)) == ARG_MATCH) {
			_debug++;
		} else if ((res = cmp_arg(argv, &i, "--psize", "-s", 0)) == ARG_MATCH) {
			arg_print_sizes = 1;
		} else if ((res = cmp_arg(argv, &i, "--ivalext", "-e", &initvals_fn_extension)) == ARG_MATCH) {
			/* initvals_fn_extension is set to the extension. */
		} else if ((res = cmp_arg(argv, &i, "--__real_infile", 0, &real_infile_name)) == ARG_MATCH) {
			/* real_infile_name is set. */
		} else {
			fprintf(stderr, "Unrecognized argument: %s\n", argv[i]);
			goto out_usage;
		}
	}
	if (!real_infile_name)
		real_infile_name = infile_name;
	if (strcmp(real_infile_name, outfile_name) == 0) {
		fprintf(stderr, "Error: INPUT and OUTPUT filename must not be the same\n");
		goto out_usage;
	}

	return 0;
out_usage:
	usage(argc, argv);
	return -1;
}

int open_input_file(void)
{
	int fd;
	int err;

	if (strcmp(infile_name, "-") == 0) {
		/* infile == stdin */
		fd = STDIN_FILENO;
	} else {
		fd = open(infile_name, O_RDONLY);
		if (fd < 0) {
			fprintf(stderr, "Could not open INPUT_FILE %s\n",
				infile_name);
			return -1;
		}
		err = dup2(fd, STDIN_FILENO);
		if (err) {
			fprintf(stderr, "Could not dup INPUT_FILE %s "
				"to STDIN\n", infile_name);
			close(fd);
			return -1;
		}
	}
	infile.fd = fd;

	return 0;
}

void close_input_file(void)
{
	if (strcmp(infile_name, "-") != 0)
		close(infile.fd);
}
