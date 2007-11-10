#ifndef BCM43xx_ASM_ARGS_H_
#define BCM43xx_ASM_ARGS_H_

int parse_args(int argc, char **argv);
int open_input_file(void);
void close_input_file(void);

extern int _debug;

#define IS_DEBUG		(_debug > 0)
#define IS_VERBOSE_DEBUG	(_debug > 1)
#define IS_INSANE_DEBUG		(_debug > 2)

#endif /* BCM43xx_ASM_ARGS_H_ */
