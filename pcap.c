#include <stdio.h>
#include <sysexits.h>
#include <string.h>
#include "pcap.h"

int
pcap_open_in(struct pcap_file *ctx, FILE * f)
{
	struct pcap_file_header h;

	if (1 != fread(&h, sizeof(h), 1, f)) {
		h.magic = 0;
	}

	if (MAGIC == h.magic) {
		ctx->swap = 0;
	} else if (bswap32(MAGIC) == h.magic) {
		ctx->swap = 1;
	} else {
		return -1;
	}
	if ((h.version_major != 2) || (h.version_minor != 4)) {
		return -1;
	}

	if (ctx->swap) {
		h.snaplen = bswap32(h.snaplen);
	}
	if (h.snaplen > MAXFRAME) {
		return -1;
	}
	ctx->linktype = h.linktype;

	ctx->f = f;
	return 0;
}

int
pcap_open_out(struct pcap_file *ctx, FILE * f)
{
	return pcap_open_out_linktype(ctx, f, 1);
}

int
pcap_open_out_linktype(struct pcap_file *ctx, FILE *f, int32_t linktype) {
	struct pcap_file_header h = { MAGIC, 2, 4, 0, 0, MAXFRAME, linktype };

	if (1 != fwrite(&h, sizeof(h), 1, f)) {
		return -1;
	}
	ctx->f = f;
	ctx->swap = 0;

	return 0;
}

int
pcap_read_pkthdr(struct pcap_file *ctx, struct pcap_pkthdr *hdr)
{
	if (1 != fread(hdr, sizeof(*hdr), 1, ctx->f)) {
		return -1;
	}

	if (ctx->swap) {
		hdr->ts.tv_sec = bswap32(hdr->ts.tv_sec);
		hdr->ts.tv_usec = bswap32(hdr->ts.tv_usec);
		hdr->caplen = bswap32(hdr->caplen);
		hdr->len = bswap32(hdr->len);
	}

	if (hdr->caplen > MAXFRAME) {
		return -1;
	}

	return 0;
}

int
pcap_write_pkthdr(struct pcap_file *ctx, struct pcap_pkthdr *hdr)
{
	if (ctx->swap) {
		struct pcap_pkthdr ohdr;

		memcpy(&ohdr, hdr, sizeof(ohdr));
		hdr->ts.tv_sec = bswap32(hdr->ts.tv_sec);
		hdr->ts.tv_usec = bswap32(hdr->ts.tv_usec);
		hdr->caplen = bswap32(hdr->caplen);
		hdr->len = bswap32(hdr->len);

		if (1 != fwrite(&ohdr, sizeof(ohdr), 1, ctx->f)) {
			return -1;
		}
	} else {
		if (1 != fwrite(hdr, sizeof(*hdr), 1, ctx->f)) {
			return -1;
		}
	}

	return 0;
}

void
pcap_close(struct pcap_file *ctx)
{
	fclose(ctx->f);
}
