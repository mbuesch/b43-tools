#ifndef BCM43xx_DASM_UTIL_H_
#define BCM43xx_DASM_UTIL_H_

#include <stdlib.h>


void dump(const char *data,
	  size_t size,
	  const char *description);


void * xmalloc(size_t size);
char * xstrdup(const char *str);
void * xrealloc(void *in, size_t size);

#endif /* BCM43xx_DASM_UTIL_H_ */
