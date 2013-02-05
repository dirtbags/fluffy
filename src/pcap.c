/*
 * PCAP library
 *
 */

#include <err.h>
#include <stdio.h>
#include <string.h>
#include "pcap.h"
#include "netre.h"

int pcap_open_in(struct pcap_file *ctx, FILE *f) {
    struct pcap_file_header h;

    if (fread(&h, sizeof(h), 1, f) != 1) h.magic = 0;

    if (MAGIC == h.magic) {
        ctx->swap = 0;
        DDUMP("normal order");
    } else if (bswap32(MAGIC) == h.magic) {
        ctx->swap = 1;
        DDUMP("swapped order");
    } else {
        DDUMP("bad magic");
        warnx("unsupported packet capture");
        return -1;
    }
    if ((h.version_major != 2) || (h.version_minor != 4)) return -1;
    DDUMP_d(h.version_major);
    DDUMP_d(h.version_minor);

    if (ctx->swap != 0) h.snaplen = bswap32(h.snaplen);
    DDUMP_d(h.snaplen);
    if (h.snaplen > MAXFRAME) {
        DDUMP("bad snaplen");
        warnx("PCAP snaplength too large (%d > %d)", h.snaplen, MAXFRAME);
        return -1;
    }

    DDUMP_x(h.linktype);
    if (h.linktype == 101) {
        ctx->raw = 1;
    } else {
        ctx->raw = 0;
    }

    ctx->f = f;
    return 0;
}

int pcap_open_out(struct pcap_file *ctx, FILE *f) {
    struct pcap_file_header h = { MAGIC, 2, 4, 0, 0, MAXFRAME, 1 };

    if (fwrite(&h, sizeof(h), 1, f) != 1) {
        warn("unable to open file");
        return -1;
    }

    ctx->f = f;
    ctx->swap = 0;

    return 0;
}

int pcap_read_pkthdr(struct pcap_file *ctx, struct pcap_pkthdr *hdr) {
    if (fread(hdr, sizeof(*hdr), 1, ctx->f) != 1) return -1;

    if (ctx->swap) {
        hdr->ts.tv_sec  = bswap32(hdr->ts.tv_sec);
        hdr->ts.tv_usec = bswap32(hdr->ts.tv_usec);
        hdr->caplen     = bswap32(hdr->caplen);
        hdr->len        = bswap32(hdr->len);
    }

    if (hdr->caplen > MAXFRAME) {
        DDUMP("bad snaplen");
        warnx("packet header capture length too large (%d > %d)", hdr->caplen, MAXFRAME);
        return -1;
    }

    return 0;
}

int pcap_write_pkthdr(struct pcap_file *ctx, struct pcap_pkthdr *hdr) {
    if (ctx->swap) {
        struct pcap_pkthdr ohdr;

        memcpy(&ohdr, hdr, sizeof(ohdr));
        hdr->ts.tv_sec  = bswap32(hdr->ts.tv_sec);
        hdr->ts.tv_usec = bswap32(hdr->ts.tv_usec);
        hdr->caplen     = bswap32(hdr->caplen);
        hdr->len        = bswap32(hdr->len);

        if (fwrite(&ohdr, sizeof(ohdr), 1, ctx->f) != 1) {
            warn("failed to write packet header");
            return -1;
        }
    } else {
        if (fwrite(hdr, sizeof(*hdr), 1, ctx->f) != 1) {
            warn("failed to write packet header");
            return -1;
        }
    }

    return 0;
}

void pcap_close(struct pcap_file *ctx) { fclose(ctx->f); }
