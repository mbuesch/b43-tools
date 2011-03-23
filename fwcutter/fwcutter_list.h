/*
 * Extraction of core 4 firmware from V4 drivers has been commented out as these
 * cores will not work with driver b43.
 *
 * In contrast, extraction of core 5 firmware from V3 drivers should be retained
 * as those devices will work with b43legacy and are useful for testing.
 */

/* file member lists */
static struct extract _e08665c5c5b66beb9c3b2dd54aa80cb3[] =
{
	{ .name = "ucode2", .offset = 0x59ca0, .length = 0x3fe0, .type = EXT_UCODE_1, },
	{ .name = "ucode4", .offset = 0x5dc84, .length = 0x4e78, .type = EXT_UCODE_1, },
	{ .name = "ucode5", .offset = 0x62b00, .length = 0x5700, .type = EXT_UCODE_2, },
	{ .name = "ucode11", .offset = 0x68204, .length = 0x54a8, .type = EXT_UCODE_2, },
	{ .name = "pcm4", .offset = 0x6d6b0, .length = 0x520, .type = EXT_PCM, },
	{ .name = "pcm5", .offset = 0x6dbd4, .length = 0x520, .type = EXT_PCM, },
	{ .name = "a0g0bsinitvals2", .offset = 0x54ad0 + 0x30d8, .length = 0x18 - 8, .type = EXT_IV, },
	{ .name = "b0g0bsinitvals5", .offset = 0x54ad0 + 0x3ae0, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "a0g0initvals5", .offset = 0x54ad0 + 0x3be0, .length = 0x9f0 - 8, .type = EXT_IV, },
	{ .name = "a0g1bsinitvals5", .offset = 0x54ad0 + 0x50c0, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "a0g0initvals2", .offset = 0x54ad0 + 0x2320, .length = 0xdb8 - 8, .type = EXT_IV, },
	{ .name = "a0g1initvals5", .offset = 0x54ad0 + 0x45d0, .length = 0x9f0 - 8, .type = EXT_IV, },
	{ .name = "b0g0bsinitvals2", .offset = 0x54ad0 + 0x2308, .length = 0x18 - 8, .type = EXT_IV, },
	{ .name = "b0g0initvals5", .offset = 0x54ad0 + 0x30f0, .length = 0x9f0 - 8, .type = EXT_IV, },
	{ .name = "b0g0initvals2", .offset = 0x54ad0 + 0x1550, .length = 0xdb8 - 8, .type = EXT_IV, },
	{ .name = "a0g0bsinitvals5", .offset = 0x54ad0 + 0x4fc0, .length = 0x100 - 8, .type = EXT_IV, },
	EXTRACT_LIST_END
};

static struct extract _9207bc565c2fc9fa1591f6c7911d3fc0[] =
{
/*
 *	{ .name = "ucode4",  .offset = 0x66220 +  0x7ad8, .length = 0x4e68, .type = EXT_UCODE_1, },
 */
	{ .name = "ucode5",  .offset = 0x66220 +  0xc944, .length = 0x5640, .type = EXT_UCODE_2, },
	{ .name = "ucode11", .offset = 0x66220 + 0x11f90, .length = 0x67e0, .type = EXT_UCODE_2, },
	{ .name = "ucode13", .offset = 0x66220 + 0x18774, .length = 0x5f60, .type = EXT_UCODE_2, },
/*
 *	{ .name = "pcm4", .offset = 0x66220 + 0x1e6d8, .length = 0x520, .type = EXT_PCM, },
 */
	{ .name = "pcm5", .offset = 0x66220 + 0x1ebfc, .length = 0x520, .type = EXT_PCM, },
/*
 *	{ .name = "b0g0initvals4",	.offset = 0x66220 + 0x1710, .length = 0xe90 - 8, .type = EXT_IV, },
 *	{ .name = "b0g0bsinitvals4",	.offset = 0x66220 + 0x25a0, .length = 0x18 - 8, .type = EXT_IV, },
 *	{ .name = "a0g0initvals4",	.offset = 0x66220 + 0x25b8, .length = 0xe90 - 8, .type = EXT_IV, },
 *	{ .name = "a0g0bsinitvals4",	.offset = 0x66220 + 0x3448, .length = 0x18 - 8, .type = EXT_IV, },
 */
	{ .name = "b0g0initvals5",	.offset = 0x66220 + 0x3460, .length = 0xa28 - 8, .type = EXT_IV, },
	{ .name = "b0g0bsinitvals5",	.offset = 0x66220 + 0x3e88, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "a0g0initvals5",	.offset = 0x66220 + 0x3f88, .length = 0xa28 - 8, .type = EXT_IV, },
	{ .name = "a0g1initvals5",	.offset = 0x66220 + 0x49b0, .length = 0xa28 - 8, .type = EXT_IV, },
	{ .name = "a0g0bsinitvals5",	.offset = 0x66220 + 0x53d8, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "a0g1bsinitvals5",	.offset = 0x66220 + 0x54d8, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "lp0initvals13",	.offset = 0x66220 + 0x5620, .length = 0xb38 - 8, .type = EXT_IV, },
	{ .name = "lp0bsinitvals13",	.offset = 0x66220 + 0x6158, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "b0g0initvals13",	.offset = 0x66220 + 0x6258, .length = 0xb40 - 8, .type = EXT_IV, },
	{ .name = "b0g0bsinitvals13",.offset = 0x66220 + 0x6d98, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "a0g1initvals13",	.offset = 0x66220 + 0x6e98, .length = 0xb40 - 8, .type = EXT_IV, },
	{ .name = "a0g1bsinitvals13",.offset = 0x66220 + 0x79d8, .length = 0x100 - 8, .type = EXT_IV, },
	EXTRACT_LIST_END
};

static struct extract _722e2e0d8cc04b8f118bb5afe6829ff9[] =
{
/*
 *	{ .name = "ucode4",  .offset = 0x76a10 +  0x8960, .length = 0x4e68, .type = EXT_UCODE_1, },
 */
	{ .name = "ucode5",  .offset = 0x76a10 +  0xd7cc, .length = 0x5640, .type = EXT_UCODE_2, },
	{ .name = "ucode11", .offset = 0x76a10 + 0x12e18, .length = 0x67e0, .type = EXT_UCODE_2, },
	{ .name = "ucode13", .offset = 0x76a10 + 0x195fc, .length = 0x5f60, .type = EXT_UCODE_2, },
/*
 *	{ .name = "pcm4", .offset = 0x76a10 + 0x1f560, .length = 0x520, .type = EXT_PCM, },
 */
	{ .name = "pcm5", .offset = 0x76a10 + 0x1fa84, .length = 0x520, .type = EXT_PCM, },
/*
 *	{ .name = "b0g0initvals4",	.offset = 0x76a10 + 0x1840, .length = 0xe90 - 8, .type = EXT_IV, },
 *	{ .name = "b0g0bsinitvals4",	.offset = 0x76a10 + 0x26d0, .length = 0x18 - 8, .type = EXT_IV, },
 *	{ .name = "a0g0initvals4",	.offset = 0x76a10 + 0x26e8, .length = 0xe90 - 8, .type = EXT_IV, },
 *	{ .name = "a0g0bsinitvals4",	.offset = 0x76a10 + 0x3578, .length = 0x18 - 8, .type = EXT_IV, },
 */
	{ .name = "b0g0initvals5",	.offset = 0x76a10 + 0x3590, .length = 0xa28 - 8, .type = EXT_IV, },
	{ .name = "b0g0bsinitvals5",	.offset = 0x76a10 + 0x3fb8, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "a0g0initvals5",	.offset = 0x76a10 + 0x40b8, .length = 0xa28 - 8, .type = EXT_IV, },
	{ .name = "a0g1initvals5",	.offset = 0x76a10 + 0x4ae0, .length = 0xa28 - 8, .type = EXT_IV, },
	{ .name = "a0g0bsinitvals5",	.offset = 0x76a10 + 0x5508, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "a0g1bsinitvals5",	.offset = 0x76a10 + 0x5608, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "lp0initvals13",	.offset = 0x76a10 + 0x64a8, .length = 0xb38 - 8, .type = EXT_IV, },
	{ .name = "lp0bsinitvals13",	.offset = 0x76a10 + 0x6fe0, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "b0g0initvals13",	.offset = 0x76a10 + 0x70e0, .length = 0xb40 - 8, .type = EXT_IV, },
	{ .name = "b0g0bsinitvals13",.offset = 0x76a10 + 0x7c20, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "a0g1initvals13",	.offset = 0x76a10 + 0x7d20, .length = 0xb40 - 8, .type = EXT_IV, },
	{ .name = "a0g1bsinitvals13",.offset = 0x76a10 + 0x8860, .length = 0x100 - 8, .type = EXT_IV, },
	EXTRACT_LIST_END
};

static struct extract _1e4763b4cb8cfbaae43e5c6d3d6b2ae7[] =
{
	{ .name = "ucode5",  .offset = 0x71c80 +  0xacd0, .length = 0x5768, .type = EXT_UCODE_2, },
	{ .name = "ucode9",  .offset = 0x71c80 + 0x1043c, .length = 0x6240, .type = EXT_UCODE_2, },
	{ .name = "ucode11", .offset = 0x71c80 + 0x16680, .length = 0x74a0, .type = EXT_UCODE_2, },
	{ .name = "ucode13", .offset = 0x71c80 + 0x1db24, .length = 0x7de0, .type = EXT_UCODE_2, },
	{ .name = "ucode14", .offset = 0x71c80 + 0x25908, .length = 0x7a90, .type = EXT_UCODE_2, },
	{ .name = "ucode15", .offset = 0x71c80 + 0x2d39c, .length = 0x7710, .type = EXT_UCODE_3, },
	{ .name = "pcm5", .offset = 0x71c80 + 0x34ab0, .length = 0x520, .type = EXT_PCM, },
	{ .name = "b0g0initvals5",	.offset = 0x71c80 + 0x14d0, .length = 0xa10 - 8, .type = EXT_IV, },
	{ .name = "b0g0bsinitvals5",	.offset = 0x71c80 + 0x1ee0, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "a0g0initvals5",	.offset = 0x71c80 + 0x1fe0, .length = 0xa10 - 8, .type = EXT_IV, },
	{ .name = "a0g1initvals5",	.offset = 0x71c80 + 0x29f0, .length = 0xa10 - 8, .type = EXT_IV, },
	{ .name = "a0g0bsinitvals5",	.offset = 0x71c80 + 0x3400, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "a0g1bsinitvals5",	.offset = 0x71c80 + 0x3500, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "b0g0initvals9",	.offset = 0x71c80 + 0x3600, .length = 0xae8 - 8, .type = EXT_IV, },
	{ .name = "b0g0bsinitvals9",	.offset = 0x71c80 + 0x40e8, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "a0g0initvals9",	.offset = 0x71c80 + 0x41e8, .length = 0xae8 - 8, .type = EXT_IV, },
	{ .name = "a0g1initvals9",	.offset = 0x71c80 + 0x4cd0, .length = 0xae8 - 8, .type = EXT_IV, },
	{ .name = "a0g0bsinitvals9",	.offset = 0x71c80 + 0x57b8, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "a0g1bsinitvals9",	.offset = 0x71c80 + 0x58b8, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "n0initvals11",	.offset = 0x71c80 + 0x59b8, .length = 0xb78 - 8, .type = EXT_IV, },
	{ .name = "n0bsinitvals11",	.offset = 0x71c80 + 0x6530, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "n0absinitvals11",	.offset = 0x71c80 + 0x6630, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "lp0initvals13",	.offset = 0x71c80 + 0x6730, .length = 0x1360 - 8, .type = EXT_IV, },
	{ .name = "lp0bsinitvals13",	.offset = 0x71c80 + 0x7a90, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "b0g0initvals13",	.offset = 0x71c80 + 0x7b90, .length = 0xb60 - 8, .type = EXT_IV, },
	{ .name = "b0g0bsinitvals13",.offset = 0x71c80 + 0x86f0, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "a0g1initvals13",	.offset = 0x71c80 + 0x87f0, .length = 0xb60 - 8, .type = EXT_IV, },
	{ .name = "a0g1bsinitvals13",.offset = 0x71c80 + 0x9350, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "lp0initvals14",	.offset = 0x71c80 + 0x9450, .length = 0xb48 - 8, .type = EXT_IV, },
	{ .name = "lp0bsinitvals14",	.offset = 0x71c80 + 0x9f98, .length = 0x100 - 8, .type = EXT_IV, },
	{ .name = "lp0initvals15",	.offset = 0x71c80 + 0xa098, .length = 0xb38 - 8, .type = EXT_IV, },
	{ .name = "lp0bsinitvals15",	.offset = 0x71c80 + 0xabd0, .length = 0x100 - 8, .type = EXT_IV, },
	EXTRACT_LIST_END
};


static struct extract _cb8d70972b885b1f8883b943c0261a3c[] =
{
	{ .name = "pcm5", .offset = 0x8e554, .type = EXT_PCM, .length = 0x520 },
/*
 *	{ .name = "pcm4", .offset = 0x8ea78, .type = EXT_PCM, .length = 0x520 },
 */
	{ .name = "ucode15", .offset = 0x8ef9c, .type = EXT_UCODE_3, .length = 0x7710 },
	{ .name = "ucode14", .offset = 0x966b0, .type = EXT_UCODE_2, .length = 0x7a90 },
	{ .name = "ucode13", .offset = 0x9e144, .type = EXT_UCODE_2, .length = 0x7de0 },
	{ .name = "ucode11", .offset = 0xa5f28, .type = EXT_UCODE_2, .length = 0x74a0 },
	{ .name = "ucode9", .offset = 0xad3cc, .type = EXT_UCODE_2, .length = 0x6240 },
	{ .name = "ucode5", .offset = 0xb3610, .type = EXT_UCODE_2, .length = 0x5768 },
/*
 *	{ .name = "ucode4", .offset = 0xb8d7c, .type = EXT_UCODE_1, .length = 0x4ec8 },
 */
	{ .name = "lp0bsinitvals15", .offset = 0xbdc44, .type = EXT_IV, .length = 0xf8 },
	{ .name = "lp0initvals15", .offset = 0xbdd44, .type = EXT_IV, .length = 0xb30 },
	{ .name = "lp0bsinitvals14", .offset = 0xbe87c, .type = EXT_IV, .length = 0xf8 },
	{ .name = "lp0initvals14", .offset = 0xbe97c, .type = EXT_IV, .length = 0xb40 },
	{ .name = "a0g1bsinitvals13", .offset = 0xbf4c4, .type = EXT_IV, .length = 0xf8 },
	{ .name = "a0g1initvals13", .offset = 0xbf5c4, .type = EXT_IV, .length = 0xb58 },
	{ .name = "b0g0bsinitvals13", .offset = 0xc0124, .type = EXT_IV, .length = 0xf8 },
	{ .name = "b0g0initvals13", .offset = 0xc0224, .type = EXT_IV, .length = 0xb58 },
	{ .name = "lp0bsinitvals13", .offset = 0xc0d84, .type = EXT_IV, .length = 0xf8 },
	{ .name = "lp0initvals13", .offset = 0xc0e84, .type = EXT_IV, .length = 0x1358 },
	{ .name = "n0absinitvals11", .offset = 0xc21e4, .type = EXT_IV, .length = 0xf8 },
	{ .name = "n0bsinitvals11", .offset = 0xc22e4, .type = EXT_IV, .length = 0xf8 },
	{ .name = "n0initvals11", .offset = 0xc23e4, .type = EXT_IV, .length = 0xb70 },
	{ .name = "a0g1bsinitvals9", .offset = 0xc2f5c, .type = EXT_IV, .length = 0xf8 },
	{ .name = "a0g0bsinitvals9", .offset = 0xc305c, .type = EXT_IV, .length = 0xf8 },
	{ .name = "a0g1initvals9", .offset = 0xc315c, .type = EXT_IV, .length = 0xae0 },
	{ .name = "a0g0initvals9", .offset = 0xc3c44, .type = EXT_IV, .length = 0xae0 },
	{ .name = "b0g0bsinitvals9", .offset = 0xc472c, .type = EXT_IV, .length = 0xf8 },
	{ .name = "b0g0initvals9", .offset = 0xc482c, .type = EXT_IV, .length = 0xae0 },
	{ .name = "a0g1bsinitvals5", .offset = 0xc5314, .type = EXT_IV, .length = 0xf8 },
	{ .name = "a0g0bsinitvals5", .offset = 0xc5414, .type = EXT_IV, .length = 0xf8 },
	{ .name = "a0g1initvals5", .offset = 0xc5514, .type = EXT_IV, .length = 0xa08 },
	{ .name = "a0g0initvals5", .offset = 0xc5f24, .type = EXT_IV, .length = 0xa08 },
	{ .name = "b0g0bsinitvals5", .offset = 0xc6934, .type = EXT_IV, .length = 0xf8 },
	{ .name = "b0g0initvals5", .offset = 0xc6a34, .type = EXT_IV, .length = 0xa08 },
/*
 *	{ .name = "a0g0bsinitvals4", .offset = 0xc7444, .type = EXT_IV, .length = 0x10 },
 *	{ .name = "a0g0initvals4", .offset = 0xc745c, .type = EXT_IV, .length = 0xe88 },
 *	{ .name = "b0g0bsinitvals4", .offset = 0xc82ec, .type = EXT_IV, .length = 0x10 },
 *	{ .name = "b0g0initvals4", .offset = 0xc8304, .type = EXT_IV, .length = 0xe8c },
 */
	EXTRACT_LIST_END
};

static struct extract _2dd738b8feb8b3559fd9d8fbaf3bfffc[] =
{
	{ .name = "lp0initvals14", .offset = 0x982B0, .type = EXT_IV, .length = 0xB80 },
	{ .name = "lp0bsinitvals14", .offset = 0x98E38, .type = EXT_IV, .length = 0x118 },
	{ .name = "b0g0initvals9", .offset = 0x92218, .type = EXT_IV, .length = 0xAF0 },
	{ .name = "a0g1bsinitvals9", .offset = 0x94540, .type = EXT_IV, .length = 0x118 },
	{ .name = "b0g0bsinitvals13", .offset = 0x974D0, .type = EXT_IV, .length = 0x118 },
	{ .name = "b0g0bsinitvals5", .offset = 0x90A78, .type = EXT_IV, .length = 0x118 },
	{ .name = "ucode4", .offset = 0x9B868, .type = EXT_UCODE_1, .length = 0x4EA0 },
	{ .name = "b0g0initvals4", .offset = 0x8E2D8, .type = EXT_IV, .length = 0xE80 },
	{ .name = "b0g0initvals13", .offset = 0x96930, .type = EXT_IV, .length = 0xB98 },
	{ .name = "ucode14", .offset = 0xBBB1C, .type = EXT_UCODE_2, .length = 0x7910 },
	{ .name = "a0g0initvals5", .offset = 0x90B98, .type = EXT_IV, .length = 0xA18 },
	{ .name = "lp0bsinitvals16", .offset = 0x9B748, .type = EXT_IV, .length = 0x118 },
	{ .name = "pcm4", .offset = 0xD4A40, .type = EXT_PCM, .length = 0x520 },
	{ .name = "a0g1bsinitvals5", .offset = 0x920F8, .type = EXT_IV, .length = 0x118 },
	{ .name = "n0bsinitvals11", .offset = 0x95230, .type = EXT_IV, .length = 0x118 },
	{ .name = "n0absinitvals11", .offset = 0x95350, .type = EXT_IV, .length = 0x118 },
	/* ERROR: Could not guess data type for: ucode_bomminor */
	{ .name = "a0g1bsinitvals13", .offset = 0x98190, .type = EXT_IV, .length = 0x118 },
	{ .name = "pcm5", .offset = 0xD4F64, .type = EXT_PCM, .length = 0x520 },
	/* ERROR: Could not guess data type for: ucode_2w_bomminor */
	{ .name = "ucode9", .offset = 0xA5E00, .type = EXT_UCODE_2, .length = 0x6248 },
	{ .name = "a0g0bsinitvals9", .offset = 0x94420, .type = EXT_IV, .length = 0x118 },
	{ .name = "a0g0bsinitvals4", .offset = 0x90020, .type = EXT_IV, .length = 0x30 },
	{ .name = "a0g1initvals5", .offset = 0x915B8, .type = EXT_IV, .length = 0xA18 },
	{ .name = "n0bsinitvals16", .offset = 0x9A980, .type = EXT_IV, .length = 0x118 },
	{ .name = "ucode16", .offset = 0xCBB9C, .type = EXT_UCODE_3, .length = 0x8EA0 },
	{ .name = "b0g0bsinitvals4", .offset = 0x8F160, .type = EXT_IV, .length = 0x30 },
	{ .name = "lp0initvals15", .offset = 0x98F58, .type = EXT_IV, .length = 0xC68 },
	{ .name = "b0g0initvals5", .offset = 0x90058, .type = EXT_IV, .length = 0xA18 },
	{ .name = "a0g0initvals4", .offset = 0x8F198, .type = EXT_IV, .length = 0xE80 },
	{ .name = "sslpn0initvals16", .offset = 0x9AAA0, .type = EXT_IV, .length = 0x0 },
	{ .name = "a0g1initvals13", .offset = 0x975F0, .type = EXT_IV, .length = 0xB98 },
	/* ERROR: Could not guess data type for: ucode_2w11 */
	/* ERROR: Could not guess data type for: ucode_bommajor */
	{ .name = "a0g1initvals9", .offset = 0x93928, .type = EXT_IV, .length = 0xAF0 },
	{ .name = "ucode5", .offset = 0xA070C, .type = EXT_UCODE_2, .length = 0x56F0 },
	{ .name = "lp0bsinitvals13", .offset = 0x96810, .type = EXT_IV, .length = 0x118 },
	{ .name = "n0initvals16", .offset = 0x99CE8, .type = EXT_IV, .length = 0xC90 },
	/* ERROR: Could not guess data type for: ucode_2w_bommajor */
	{ .name = "b0g0bsinitvals9", .offset = 0x92D10, .type = EXT_IV, .length = 0x118 },
	{ .name = "ucode11", .offset = 0xAC04C, .type = EXT_UCODE_2, .length = 0x8000 },
	{ .name = "lp0initvals16", .offset = 0x9AAB0, .type = EXT_IV, .length = 0xC90 },
	/* ERROR: Could not guess data type for: ucode_2w15 */
	{ .name = "a0g0initvals9", .offset = 0x92E30, .type = EXT_IV, .length = 0xAF0 },
	{ .name = "lp0initvals13", .offset = 0x95470, .type = EXT_IV, .length = 0x1398 },
	{ .name = "a0g0bsinitvals5", .offset = 0x91FD8, .type = EXT_IV, .length = 0x118 },
	{ .name = "ucode13", .offset = 0xB4050, .type = EXT_UCODE_2, .length = 0x7AC8 },
	/* ERROR: Could not guess data type for: ucode_2w13 */
	{ .name = "ucode15", .offset = 0xC3430, .type = EXT_UCODE_3, .length = 0x8768 },
	{ .name = "lp0bsinitvals15", .offset = 0x99BC8, .type = EXT_IV, .length = 0x118 },
	{ .name = "n0initvals11", .offset = 0x94660, .type = EXT_IV, .length = 0xBC8 },
	{ .name = "sslpn0bsinitvals16", .offset = 0x9AAA8, .type = EXT_IV, .length = 0x0 },
	EXTRACT_LIST_END
};

static struct extract _1a258b2d93efa641c32ddf3c3a962028[] =
{
	/* { .name = "ucode4", .offset = 0xAFE08, .type = EXT_UCODE_1, .length = 0x4EA0 }, */
	/* { .name = "pcm4", .offset = 0xE8FE0, .type = EXT_PCM, .length = 0x520 }, */
	/* { .name = "b0g0initvals4", .offset = 0xA2878, .type = EXT_IV, .length = 0xE80 }, */
	/* { .name = "a0g0bsinitvals4", .offset = 0xA45C0, .type = EXT_IV, .length = 0x30 }, */
	/* { .name = "b0g0bsinitvals4", .offset = 0xA3700, .type = EXT_IV, .length = 0x30 }, */
	/* { .name = "a0g0initvals4", .offset = 0xA3738, .type = EXT_IV, .length = 0xE80 }, */
	{ .name = "ucode5", .offset = 0xB4CAC, .type = EXT_UCODE_2, .length = 0x56F0 },
	{ .name = "pcm5", .offset = 0xE9504, .type = EXT_PCM, .length = 0x520 },
	{ .name = "a0g0bsinitvals5", .offset = 0xA6578, .type = EXT_IV, .length = 0x118 },
	{ .name = "b0g0bsinitvals5", .offset = 0xA5018, .type = EXT_IV, .length = 0x118 },
	{ .name = "a0g0initvals5", .offset = 0xA5138, .type = EXT_IV, .length = 0xA18 },
	{ .name = "a0g1initvals5", .offset = 0xA5B58, .type = EXT_IV, .length = 0xA18 },
	{ .name = "b0g0initvals5", .offset = 0xA45F8, .type = EXT_IV, .length = 0xA18 },
	{ .name = "a0g1bsinitvals5", .offset = 0xA6698, .type = EXT_IV, .length = 0x118 },
	{ .name = "ucode9", .offset = 0xBA3A0, .type = EXT_UCODE_2, .length = 0x6248 },
	{ .name = "a0g0bsinitvals9", .offset = 0xA89C0, .type = EXT_IV, .length = 0x118 },
	{ .name = "b0g0initvals9", .offset = 0xA67B8, .type = EXT_IV, .length = 0xAF0 },
	{ .name = "a0g1bsinitvals9", .offset = 0xA8AE0, .type = EXT_IV, .length = 0x118 },
	{ .name = "b0g0bsinitvals9", .offset = 0xA72B0, .type = EXT_IV, .length = 0x118 },
	{ .name = "a0g0initvals9", .offset = 0xA73D0, .type = EXT_IV, .length = 0xAF0 },
	{ .name = "a0g1initvals9", .offset = 0xA7EC8, .type = EXT_IV, .length = 0xAF0 },
	{ .name = "ucode11", .offset = 0xC05EC, .type = EXT_UCODE_2, .length = 0x8000 },
	{ .name = "n0initvals11", .offset = 0xA8C00, .type = EXT_IV, .length = 0xBC8 },
	{ .name = "n0bsinitvals11", .offset = 0xA97D0, .type = EXT_IV, .length = 0x118 },
	{ .name = "n0absinitvals11", .offset = 0xA98F0, .type = EXT_IV, .length = 0x118 },
	{ .name = "ucode13", .offset = 0xC85F0, .type = EXT_UCODE_2, .length = 0x7AC8 },
	{ .name = "b0g0bsinitvals13", .offset = 0xABA70, .type = EXT_IV, .length = 0x118 },
	{ .name = "b0g0initvals13", .offset = 0xAAED0, .type = EXT_IV, .length = 0xB98 },
	{ .name = "a0g1bsinitvals13", .offset = 0xAC730, .type = EXT_IV, .length = 0x118 },
	{ .name = "a0g1initvals13", .offset = 0xABB90, .type = EXT_IV, .length = 0xB98 },
	{ .name = "lp0bsinitvals13", .offset = 0xAADB0, .type = EXT_IV, .length = 0x118 },
	{ .name = "lp0initvals13", .offset = 0xA9A10, .type = EXT_IV, .length = 0x1398 },
	{ .name = "ucode14", .offset = 0xD00BC, .type = EXT_UCODE_2, .length = 0x7910 },
	{ .name = "lp0initvals14", .offset = 0xAC850, .type = EXT_IV, .length = 0xB80 },
	{ .name = "lp0bsinitvals14", .offset = 0xAD3D8, .type = EXT_IV, .length = 0x118 },
	{ .name = "ucode15", .offset = 0xD79D0, .type = EXT_UCODE_3, .length = 0x8768 },
	{ .name = "lp0initvals15", .offset = 0xAD4F8, .type = EXT_IV, .length = 0xC68 },
	{ .name = "lp0bsinitvals15", .offset = 0xAE168, .type = EXT_IV, .length = 0x118 },
	{ .name = "ucode16", .offset = 0xE013C, .type = EXT_UCODE_3, .length = 0x8EA0 },
	{ .name = "lp0bsinitvals16", .offset = 0xAFCE8, .type = EXT_IV, .length = 0x118 },
	{ .name = "n0bsinitvals16", .offset = 0xAEF20, .type = EXT_IV, .length = 0x118 },
	{ .name = "sslpn0initvals16", .offset = 0xAF040, .type = EXT_IV, .length = 0x0 },
	{ .name = "n0initvals16", .offset = 0xAE288, .type = EXT_IV, .length = 0xC90 },
	{ .name = "lp0initvals16", .offset = 0xAF050, .type = EXT_IV, .length = 0xC90 },
	{ .name = "sslpn0bsinitvals16", .offset = 0xAF048, .type = EXT_IV, .length = 0x0 },
	EXTRACT_LIST_END
};

static struct extract _bb8537e3204a1ea5903fe3e66b5e2763[] =
{
	/* ucode major version at offset 0xa8b70 */
	/* ucode minor version at offset 0xa8b74 */
	/* { .name = "ucode4", .offset = 0xB6108, .type = EXT_UCODE_1, .length = 0x4EA0 }, */
	/* { .name = "pcm4", .offset = 0xEF2E0, .type = EXT_PCM, .length = 0x520 }, */
	/* { .name = "b0g0initvals4", .offset = 0xA8B78, .type = EXT_IV, .length = 0xE80 }, */
	/* { .name = "a0g0bsinitvals4", .offset = 0xAA8C0, .type = EXT_IV, .length = 0x30 }, */
	/* { .name = "b0g0bsinitvals4", .offset = 0xA9A00, .type = EXT_IV, .length = 0x30 }, */
	/* { .name = "a0g0initvals4", .offset = 0xA9A38, .type = EXT_IV, .length = 0xE80 }, */
	{ .name = "ucode5", .offset = 0xBAFAC, .type = EXT_UCODE_2, .length = 0x56F0 },
	{ .name = "pcm5", .offset = 0xEF804, .type = EXT_PCM, .length = 0x520 },
	{ .name = "b0g0bsinitvals5", .offset = 0xAB318, .type = EXT_IV, .length = 0x118 },
	{ .name = "a0g0bsinitvals5", .offset = 0xAC878, .type = EXT_IV, .length = 0x118 },
	{ .name = "b0g0initvals5", .offset = 0xAA8F8, .type = EXT_IV, .length = 0xA18 },
	{ .name = "a0g1initvals5", .offset = 0xABE58, .type = EXT_IV, .length = 0xA18 },
	{ .name = "a0g0initvals5", .offset = 0xAB438, .type = EXT_IV, .length = 0xA18 },
	{ .name = "a0g1bsinitvals5", .offset = 0xAC998, .type = EXT_IV, .length = 0x118 },
	{ .name = "ucode9", .offset = 0xC06A0, .type = EXT_UCODE_2, .length = 0x6248 },
	{ .name = "a0g1initvals9", .offset = 0xAE1C8, .type = EXT_IV, .length = 0xAF0 },
	{ .name = "a0g0bsinitvals9", .offset = 0xAECC0, .type = EXT_IV, .length = 0x118 },
	{ .name = "b0g0bsinitvals9", .offset = 0xAD5B0, .type = EXT_IV, .length = 0x118 },
	{ .name = "b0g0initvals9", .offset = 0xACAB8, .type = EXT_IV, .length = 0xAF0 },
	{ .name = "a0g1bsinitvals9", .offset = 0xAEDE0, .type = EXT_IV, .length = 0x118 },
	{ .name = "a0g0initvals9", .offset = 0xAD6D0, .type = EXT_IV, .length = 0xAF0 },
	{ .name = "ucode11", .offset = 0xC68EC, .type = EXT_UCODE_2, .length = 0x8000 },
	{ .name = "n0bsinitvals11", .offset = 0xAFAD0, .type = EXT_IV, .length = 0x118 },
	{ .name = "n0absinitvals11", .offset = 0xAFBF0, .type = EXT_IV, .length = 0x118 },
	{ .name = "n0initvals11", .offset = 0xAEF00, .type = EXT_IV, .length = 0xBC8 },
	{ .name = "ucode13", .offset = 0xCE8F0, .type = EXT_UCODE_2, .length = 0x7AC8 },
	{ .name = "b0g0initvals13", .offset = 0xB11D0, .type = EXT_IV, .length = 0xB98 },
	{ .name = "a0g1bsinitvals13", .offset = 0xB2A30, .type = EXT_IV, .length = 0x118 },
	{ .name = "a0g1initvals13", .offset = 0xB1E90, .type = EXT_IV, .length = 0xB98 },
	{ .name = "lp0bsinitvals13", .offset = 0xB10B0, .type = EXT_IV, .length = 0x118 },
	{ .name = "b0g0bsinitvals13", .offset = 0xB1D70, .type = EXT_IV, .length = 0x118 },
	{ .name = "lp0initvals13", .offset = 0xAFD10, .type = EXT_IV, .length = 0x1398 },
	{ .name = "ucode14", .offset = 0xD63BC, .type = EXT_UCODE_2, .length = 0x7910 },
	{ .name = "lp0initvals14", .offset = 0xB2B50, .type = EXT_IV, .length = 0xB80 },
	{ .name = "lp0bsinitvals14", .offset = 0xB36D8, .type = EXT_IV, .length = 0x118 },
	{ .name = "ucode15", .offset = 0xDDCD0, .type = EXT_UCODE_3, .length = 0x8768 },
	{ .name = "lp0bsinitvals15", .offset = 0xB4468, .type = EXT_IV, .length = 0x118 },
	{ .name = "lp0initvals15", .offset = 0xB37F8, .type = EXT_IV, .length = 0xC68 },
	{ .name = "ucode16", .offset = 0xE643C, .type = EXT_UCODE_3, .length = 0x8EA0 },
	{ .name = "n0bsinitvals16", .offset = 0xB5220, .type = EXT_IV, .length = 0x118 },
	{ .name = "sslpn0initvals16", .offset = 0xB5340, .type = EXT_IV, .length = 0x0 },
	{ .name = "n0initvals16", .offset = 0xB4588, .type = EXT_IV, .length = 0xC90 },
	{ .name = "lp0initvals16", .offset = 0xB5350, .type = EXT_IV, .length = 0xC90 },
	{ .name = "sslpn0bsinitvals16", .offset = 0xB5348, .type = EXT_IV, .length = 0x0 },
	{ .name = "lp0bsinitvals16", .offset = 0xB5FE8, .type = EXT_IV, .length = 0x118 },
	EXTRACT_LIST_END
};

static struct extract _490d4e149ecc45eb1a91f06aa75be071[] =
{
	{ .name = "ucode19", .offset = 0xFB3BC, .type = EXT_UCODE_3, .length = 0x9998 },
	{ .name = "lp0initvals14", .offset = 0x999C8, .type = EXT_IV, .length = 0xB20 },
	{ .name = "ucode16_lp", .offset = 0xCE5F4, .type = EXT_UCODE_3, .length = 0x9D90 },
	{ .name = "ucode16_sslpn", .offset = 0xD8388, .type = EXT_UCODE_3, .length = 0x8936 },
	{ .name = "lp0bsinitvals14", .offset = 0x9A4F0, .type = EXT_IV, .length = 0x118 },
	{ .name = "b0g0initvals9", .offset = 0x939F8, .type = EXT_IV, .length = 0xAE0 },
	{ .name = "sslpn2bsinitvals17", .offset = 0x9ECE8, .type = EXT_IV, .length = 0x118 },
	{ .name = "a0g1bsinitvals9", .offset = 0x95CF0, .type = EXT_IV, .length = 0x118 },
	{ .name = "b0g0bsinitvals13", .offset = 0x98C00, .type = EXT_IV, .length = 0x118 },
	{ .name = "ucode16_sslpn_nobt", .offset = 0xE0CC4, .type = EXT_UCODE_3, .length = 0x7413 },
	{ .name = "b0g0bsinitvals5", .offset = 0x92278, .type = EXT_IV, .length = 0x118 },
	{ .name = "sslpn2initvals17", .offset = 0x9E020, .type = EXT_IV, .length = 0xCC0 },
	{ .name = "ucode4", .offset = 0xA0A68, .type = EXT_UCODE_1, .length = 0x4E80 },
	{ .name = "b0g0initvals4", .offset = 0x8FB08, .type = EXT_IV, .length = 0xE70 },
	{ .name = "b0g0initvals13", .offset = 0x98078, .type = EXT_IV, .length = 0xB80 },
	{ .name = "ucode17", .offset = 0xF1B50, .type = EXT_UCODE_3, .length = 0x9868 },
	{ .name = "sslpn1bsinitvals20", .offset = 0xA0948, .type = EXT_IV, .length = 0x118 },
	{ .name = "ucode14", .offset = 0xBF864, .type = EXT_UCODE_2, .length = 0x64A0 },
	{ .name = "a0g0initvals5", .offset = 0x92398, .type = EXT_IV, .length = 0xA08 },
	{ .name = "lp0bsinitvals16", .offset = 0x9DF00, .type = EXT_IV, .length = 0x118 },
	{ .name = "pcm4", .offset = 0x10E9EC, .type = EXT_PCM, .length = 0x520 },
	{ .name = "a0g1bsinitvals5", .offset = 0x938D8, .type = EXT_IV, .length = 0x118 },
	{ .name = "n0bsinitvals11", .offset = 0x96990, .type = EXT_IV, .length = 0x118 },
	{ .name = "n0absinitvals11", .offset = 0x96AB0, .type = EXT_IV, .length = 0x118 },
	/* ucode minor version at offset 0x8fb04 */
	{ .name = "a0g1bsinitvals13", .offset = 0x998A8, .type = EXT_IV, .length = 0x118 },
	{ .name = "pcm5", .offset = 0x10EF10, .type = EXT_PCM, .length = 0x520 },
	{ .name = "ucode9", .offset = 0xAB000, .type = EXT_UCODE_2, .length = 0x6268 },
	{ .name = "a0g0bsinitvals9", .offset = 0x95BD0, .type = EXT_IV, .length = 0x118 },
	{ .name = "a0g0bsinitvals4", .offset = 0x91830, .type = EXT_IV, .length = 0x30 },
	{ .name = "ucode20", .offset = 0x104D58, .type = EXT_UCODE_3, .length = 0x9C90 },
	{ .name = "a0g1initvals5", .offset = 0x92DA8, .type = EXT_IV, .length = 0xA08 },
	{ .name = "n0bsinitvals16", .offset = 0x9C1D0, .type = EXT_IV, .length = 0x118 },
	{ .name = "b0g0bsinitvals4", .offset = 0x90980, .type = EXT_IV, .length = 0x30 },
	{ .name = "lp0initvals15", .offset = 0x9A610, .type = EXT_IV, .length = 0xD20 },
	{ .name = "b0g0initvals5", .offset = 0x91868, .type = EXT_IV, .length = 0xA08 },
	{ .name = "a0g0initvals4", .offset = 0x909B8, .type = EXT_IV, .length = 0xE70 },
	{ .name = "sslpn0initvals16", .offset = 0x9C2F0, .type = EXT_IV, .length = 0xD70 },
	{ .name = "a0g1initvals13", .offset = 0x98D20, .type = EXT_IV, .length = 0xB80 },
	{ .name = "sslpn2initvals19", .offset = 0x9EE08, .type = EXT_IV, .length = 0xCB0 },
	/* ucode major version at offset 0x8fb00 */
	{ .name = "a0g1initvals9", .offset = 0x950E8, .type = EXT_IV, .length = 0xAE0 },
	{ .name = "ucode5", .offset = 0xA58EC, .type = EXT_UCODE_2, .length = 0x5710 },
	{ .name = "lp0bsinitvals13", .offset = 0x97F58, .type = EXT_IV, .length = 0x118 },
	{ .name = "n0initvals16", .offset = 0x9B458, .type = EXT_IV, .length = 0xD70 },
	{ .name = "b0g0bsinitvals9", .offset = 0x944E0, .type = EXT_IV, .length = 0x118 },
	{ .name = "ucode11", .offset = 0xB126C, .type = EXT_UCODE_2, .length = 0x74A8 },
	{ .name = "lp0initvals16", .offset = 0x9D188, .type = EXT_IV, .length = 0xD70 },
	{ .name = "ucode16_mimo", .offset = 0xE80DC, .type = EXT_UCODE_3, .length = 0x9A70 },
	{ .name = "a0g0initvals9", .offset = 0x94600, .type = EXT_IV, .length = 0xAE0 },
	{ .name = "lp0initvals13", .offset = 0x96BD0, .type = EXT_IV, .length = 0x1380 },
	{ .name = "a0g0bsinitvals5", .offset = 0x937B8, .type = EXT_IV, .length = 0x118 },
	{ .name = "ucode13", .offset = 0xB8718, .type = EXT_UCODE_2, .length = 0x7148 },
	{ .name = "sslpn2bsinitvals19", .offset = 0x9FAC0, .type = EXT_IV, .length = 0x118 },
	{ .name = "ucode15", .offset = 0xC5D08, .type = EXT_UCODE_3, .length = 0x88E8 },
	{ .name = "lp0bsinitvals15", .offset = 0x9B338, .type = EXT_IV, .length = 0x118 },
	{ .name = "n0initvals11", .offset = 0x95E10, .type = EXT_IV, .length = 0xB78 },
	{ .name = "sslpn0bsinitvals16", .offset = 0x9D068, .type = EXT_IV, .length = 0x118 },
	{ .name = "sslpn1initvals20", .offset = 0x9FBE0, .type = EXT_IV, .length = 0xD60 },
	EXTRACT_LIST_END
};
/*
 * Links change, so let's not put them into the README.
 * I still put them here so we know where the file was obtained.
 */
static const struct file files[] = 
{
	{
		.name		= "wl_apsta.o",
		.id		= "FW10",
		.ucode_version	= "295.14",
		.md5		= "e08665c5c5b66beb9c3b2dd54aa80cb3",
		.flags		= FW_FLAG_LE,
		.extract	= _e08665c5c5b66beb9c3b2dd54aa80cb3,
	},
	{
		/* http://downloads.openwrt.org/sources/broadcom-wl-4.80.53.0.tar.bz2 */
		/* This firmware has the old TX header. */
		.name		= "wl_apsta.o",
		.id		= "FW11",
		.ucode_version	= "351.126",
		.md5		= "9207bc565c2fc9fa1591f6c7911d3fc0",
		.flags		= FW_FLAG_LE | FW_FLAG_V4,
		.extract	= _9207bc565c2fc9fa1591f6c7911d3fc0,
	},
	{
		/* http://downloads.openwrt.org/sources/broadcom-wl-4.80.53.0.tar.bz2 */
		/* This firmware has the old TX header. */
		.name		= "wl_apsta_mimo.o",
		.id		= "FW11",
		.ucode_version	= "351.126",
		.md5		= "722e2e0d8cc04b8f118bb5afe6829ff9",
		.flags		= FW_FLAG_LE | FW_FLAG_V4,
		.extract	= _722e2e0d8cc04b8f118bb5afe6829ff9,
	},
	{
		/* ftp://ftp.linksys.com/opensourcecode/wrt150nv11/1.51.3/ */
		.name		= "wl_ap.o",
		.id		= "FW12",
		.ucode_version	= "410.2160",
		.md5		= "1e4763b4cb8cfbaae43e5c6d3d6b2ae7",
		.flags		= FW_FLAG_LE | FW_FLAG_V4 | FW_FLAG_UNSUPPORTED,
		.extract	= _1e4763b4cb8cfbaae43e5c6d3d6b2ae7,
	},
	{
		/* http://mirror2.openwrt.org/sources/broadcom-wl-4.150.10.5.tar.bz2 */
		.name		= "wl_apsta_mimo.o",
		.id		= "FW13",
		.ucode_version	= "410.2160",
		.md5		= "cb8d70972b885b1f8883b943c0261a3c",
		.flags		= FW_FLAG_LE | FW_FLAG_V4,
		.extract	= _cb8d70972b885b1f8883b943c0261a3c,
	},
	{
		/* ftp://ftp.linksys.com/opensourcecode/wrt610n/1.00.00.018/wrt610n_v1.00.00.018_us.tgz */
		.name		= "wl.o",
		.id		= "FW14",
		.ucode_version	= "478.104",
		.md5		= "2dd738b8feb8b3559fd9d8fbaf3bfffc",
		.flags		= FW_FLAG_LE | FW_FLAG_V4 | FW_FLAG_UNSUPPORTED,
		.extract	= _2dd738b8feb8b3559fd9d8fbaf3bfffc,
	},
	{
		/* ftp://downloads.netgear.com/files/GPL/WNDR3300-V1.0.29_gpl_src.zip */
		.name		= "wl_apsta.o",
		.id		= "FW15",
		.ucode_version	= "478.104",
		.md5		= "1a258b2d93efa641c32ddf3c3a962028",
		.flags		= FW_FLAG_LE | FW_FLAG_V4,
		.extract	= _1a258b2d93efa641c32ddf3c3a962028,
	},
	{
		/* http://downloads.openwrt.org/sources/broadcom-wl-4.178.10.4.tar.bz2 */
		.name		= "wl_apsta.o",
		.id		= "FW15",
		.ucode_version	= "478.104",
		.md5		= "bb8537e3204a1ea5903fe3e66b5e2763",
		.flags		= FW_FLAG_LE | FW_FLAG_V4,
		.extract	= _bb8537e3204a1ea5903fe3e66b5e2763,
	},
	{
		/* http://mirror2.openwrt.org/sources/broadcom-wl-5.10.56.27.3_mipsel.tar.bz2 */
		.name		= "wl_prebuilt.o",
		.id		= "FW15",
		.ucode_version	= "508.1084",
		.md5		= "490d4e149ecc45eb1a91f06aa75be071",
		.flags		= FW_FLAG_LE | FW_FLAG_V4,
		.extract	= _490d4e149ecc45eb1a91f06aa75be071,
	},
};
