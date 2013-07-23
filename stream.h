#ifndef _STREAM_H
#define _STREAM_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

enum endianness {
	ENDIAN_BIG,
	ENDIAN_LITTLE
};

struct stream {
	char const *buf;
	size_t len;
	enum endianness endian;
};

void sinit(struct stream *s, char const *buf, size_t len, enum endianness endian);
bool sskip(struct stream *s, size_t count);
bool sread(struct stream *s, void *buf, size_t count);
uint8_t read_uint8(struct stream *s);
uint16_t read_uint16be(struct stream *s);
uint16_t read_uint16le(struct stream *s);
uint16_t read_uint16(struct stream *s);
uint32_t read_uint32be(struct stream *s);
uint32_t read_uint32le(struct stream *s);
uint32_t read_uint32(struct stream *s);

#endif
