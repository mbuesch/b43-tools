#ifndef B43_DASM_ARGS_H_
#define B43_DASM_ARGS_H_

struct cmdline_args {
	unsigned int arch;
	int no_header;
	int print_addresses;
};

int parse_args(int argc, char **argv);

int open_input_file(void);
void close_input_file(void);
int open_output_file(void);
void close_output_file(void);

extern int _debug;
extern struct cmdline_args cmdargs;

#define IS_DEBUG		(_debug > 0)
#define IS_VERBOSE_DEBUG	(_debug > 1)
#define IS_INSANE_DEBUG		(_debug > 2)

#endif /* B43_DASM_ARGS_H_ */
