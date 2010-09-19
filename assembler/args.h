#ifndef BCM43xx_ASM_ARGS_H_
#define BCM43xx_ASM_ARGS_H_

#include "util.h"


enum fwformat {
	FMT_RAW_LE32,   /* Raw microcode. No headers. 32bit little endian chunks. */
	FMT_RAW_BE32,   /* Raw microcode. No headers. 32bit big endian chunks. */
	FMT_B43,        /* b43/b43legacy headers. */
};

int parse_args(int argc, char **argv);
int open_input_file(void);
void close_input_file(void);

extern int _debug;
extern bool arg_print_sizes;
extern const char *initvals_fn_extension;
extern enum fwformat output_format;

#define IS_DEBUG		(_debug > 0)
#define IS_VERBOSE_DEBUG	(_debug > 1)
#define IS_INSANE_DEBUG		(_debug > 2)

#endif /* BCM43xx_ASM_ARGS_H_ */
