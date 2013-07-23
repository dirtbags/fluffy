#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "stream.h"

void
sinit(struct stream *s, char const *buf, size_t buflen, enum endianness endian)
{
	s->buf = buf;
	s->len = buflen;
	s->endian = endian;
}

bool
sskip(struct stream *s, size_t count)
{
	if (count > s->len) {
		s->len = 0;
		return false;
	}

	s->buf += count;
	s->len -= count;
	return true;
}

bool
sread(struct stream *s, void *buf, size_t count)
{
	void const *d = s->buf;

	if (!sskip(s, count)) {
		return false;
	}
	memcpy(buf, d, count);

	return true;
}



uint8_t
read_uint8(struct stream *s)
{
	uint8_t *d = (uint8_t *) s->buf;

	if (!sskip(s, 1)) {
		return 0;
	}
	return d[0];
}



uint16_t
read_uint16be(struct stream *s)
{
	uint8_t *d = (uint8_t *) s->buf;

	if (!sskip(s, 2)) {
		return 0;
	}
	return ((d[0] << 8) | (d[1] << 0));
}


uint16_t
read_uint16le(struct stream *s)
{
	uint8_t *d = (uint8_t *) s->buf;

	if (!sskip(s, 2)) {
		return 0;
	}
	return ((d[0] << 0) | (d[1] << 8));
}

uint16_t 
read_uint16(struct stream *s)
{
	if (s->endian == ENDIAN_BIG) {
		return read_uint16be(s);
	} else {
		return read_uint16le(s);
	}
}


uint32_t
read_uint32be(struct stream *s)
{
	uint8_t *d = (uint8_t *) s->buf;
	if (!sskip(s, 4)) {
		return 0;
	}
	return ((d[0] << 24) | (d[1] << 16) | (d[2] << 8) | (d[3] << 0));
}

uint32_t
read_uint32le(struct stream *s)
{
	uint8_t *d = (uint8_t *) s->buf;
	if (!sskip(s, 4)) {
		return 0;
	}
	return ((d[0] << 0) | (d[1] << 8) | (d[2] << 16) | (d[3] << 24));
}

uint32_t
read_uint32(struct stream *s)
{
	if (s->endian == ENDIAN_BIG) {
		return read_uint32be(s);
	} else {
		return read_uint32le(s);
	}
}
