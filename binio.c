#include <stdint.h>
#include <string.h>
#include "binio.h"

#define rc(f, shl) (breaduint8(f) << shl)

uint8_t breaduint8(BINFILE *f) {
        int c = fgetc(f->f);
        if (EOF == c) {
                return 0;
        }
        return c;
}

uint16_t breaduint16be(BINFILE *f) {
        return rc(f, 8) | rc(f, 0);
}
uint16_t breaduint16le(BINFILE *f) {
        return rc(f, 0) | rc(f, 8);
}
uint16_t breaduint16(BINFILE *f) {
        switch (f->endian) {
        case ENDIAN_BIG:
                return breaduint16be(f);
        case ENDIAN_LITTLE:
                return breaduint16le(f);
        default:
                return 0;
        }
}

int16_t breadint16be(BINFILE *f) {
        return (int16_t)breaduint16be(f);
}
int16_t breadint16le(BINFILE *f) {
        return (int16_t)breaduint16le(f);
}
int16_t breadint16(BINFILE *f) {
        return (int16_t)breaduint16(f);
}

uint32_t breaduint32be(BINFILE *f) {
        return rc(f, 24) | rc(f, 16) | rc(f, 8) | rc(f, 0);
}
uint32_t breaduint32le(BINFILE *f) {
        return rc(f, 0) | rc(f, 8) | rc(f, 16) | rc(f, 24);
        
}
uint32_t breaduint32(BINFILE *f) {
        switch (f->endian) {
        case ENDIAN_BIG:
                return breaduint32be(f);
        case ENDIAN_LITTLE:
                return breaduint32le(f);
        default:
                return 0;
        }
}

int32_t breadint32be(BINFILE *f) {
        return (int32_t)breaduint32be(f);
}
int32_t breadint32le(BINFILE *f) {
        return (int32_t)breaduint32le(f);
}
int32_t breadint32(BINFILE *f) {
        return (int32_t)breaduint32(f);
}


#define wc(f, v, shl) bwriteuint8(f, (v >> shl) & 0xff)

int bwriteuint8(BINFILE *f, uint8_t v) {
        return fputc(v, f->f);
}

int bwriteuint16be(BINFILE *f, uint16_t v) {
        return wc(f, v, 8) | wc(f, v, 0);
}
int bwriteuint16le(BINFILE *f, uint16_t v) {
        return wc(f, v, 0) | wc(f, v, 8);
}
int bwriteuint16(BINFILE *f, uint16_t v) {
        switch (f->endian) {
        case ENDIAN_BIG:
                return bwriteuint16be(f, v);
        case ENDIAN_LITTLE:
                return bwriteuint16le(f, v);
        default:
                return -1;
        }
}

int bwriteint16be(BINFILE *f, int16_t v) {
        return bwriteuint16be(f, (uint16_t)v);
}
int bwriteint16le(BINFILE *f, int16_t v) {
        return bwriteuint16le(f, (uint16_t)v);
}
int bwriteint16(BINFILE *f, int16_t v) {
        return bwriteuint16(f, (uint16_t)v);
}

int bwriteuint32be(BINFILE *f, uint32_t v) {
        return wc(f, v, 24) | wc(f, v, 16) | wc(f, v, 8) | wc(f, v, 0);
}
int bwriteuint32le(BINFILE *f, uint32_t v) {
        return wc(f, v, 0) | wc(f, v, 8) | wc(f, v, 16) | wc(f, v, 24);
}
int bwriteuint32(BINFILE *f, uint32_t v) {
        switch (f->endian) {
        case ENDIAN_BIG:
                return bwriteuint32be(f, v);
        case ENDIAN_LITTLE:
                return bwriteuint32le(f, v);
        default:
                return -1;
        }
}

int bwriteint32be(BINFILE *f, int32_t v) {
        return bwriteuint32be(f, (uint32_t)v);
}
int bwriteint32le(BINFILE *f, int32_t v) {
        return bwriteuint32le(f, (uint32_t)v);
}
int bwriteint32(BINFILE *f, int32_t v) {
        return bwriteuint32(f, (uint32_t)v);
}


void breadskip(BINFILE *f, size_t len) {
        while (len > 0) {
                breaduint8(f);
        }
}
