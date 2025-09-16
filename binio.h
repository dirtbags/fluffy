#pragma once

#include <stdint.h>
#include <stdio.h>

enum endianness {
	ENDIAN_BIG,
	ENDIAN_LITTLE
};
#define ENDIAN_NETWORK ENDIAN_BIG

typedef struct {
        FILE *f;
        enum endianness endian;
} BINFILE;

uint8_t breaduint8(BINFILE *f);
uint16_t breaduint16be(BINFILE *f);
uint16_t breaduint16le(BINFILE *f);
uint16_t breaduint16(BINFILE *f);
int16_t breadint16be(BINFILE *f);
int16_t breadint16le(BINFILE *f);
int16_t breadint16(BINFILE *f);
uint32_t breaduint32be(BINFILE *f);
uint32_t breaduint32le(BINFILE *f);
uint32_t breaduint32(BINFILE *f);
int32_t breadint32be(BINFILE *f);
int32_t breadint32le(BINFILE *f);
int32_t breadint32(BINFILE *f);

int bwriteuint8(BINFILE *f, uint8_t v);
int bwriteuint16be(BINFILE *f, uint16_t v);
int bwriteuint16le(BINFILE *f, uint16_t v);
int bwriteuint16(BINFILE *f, uint16_t v);
int bwriteint16be(BINFILE *f, int16_t v);
int bwriteint16le(BINFILE *f, int16_t v);
int bwriteint16(BINFILE *f, int16_t v);
int bwriteuint32be(BINFILE *f, uint32_t v);
int bwriteuint32le(BINFILE *f, uint32_t v);
int bwriteuint32(BINFILE *f, uint32_t v);
int bwriteint32be(BINFILE *f, int32_t v);
int bwriteint32le(BINFILE *f, int32_t v);
int bwriteint32(BINFILE *f, int32_t v);

void breadskip(BINFILE *f, size_t n);

