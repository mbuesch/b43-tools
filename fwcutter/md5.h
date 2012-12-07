#ifndef FWCUTTER_MD5_H_
#define FWCUTTER_MD5_H_

#include <stdint.h>

struct MD5Context {
	uint32_t buf[4];
	uint32_t bits[2];
	union _u {
		unsigned char in[64];
		uint32_t in_u32[16];
	} u;
};

void MD5Init(struct MD5Context *ctx);
void MD5Update(struct MD5Context *ctx, unsigned char *buf, unsigned len);
void MD5Final(unsigned char *digest, struct MD5Context *ctx);

#endif /* FWCUTTER_MD5_H_ */
