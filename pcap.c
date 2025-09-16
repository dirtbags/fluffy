#include <stdio.h>
#include <sysexits.h>
#include <string.h>
#include "pcap.h"
#include "binio.h"

int pcap_open_in(struct pcap_file *ctx, BINFILE * f) {
	ctx->f = f;

	uint32_t magic = breaduint32be(ctx->f);
	switch (magic) {
	case PCAP_MAGIC:
		ctx->f->endian = ENDIAN_BIG;
		break;
	case PCAP_CIGAM:
		ctx->f->endian = ENDIAN_LITTLE;
		break;
	default:
		return -1;
	}

	ctx->h.version_major = breaduint16(ctx->f);
	ctx->h.version_minor = breaduint16(ctx->f);
	ctx->h.thiszone = breadint32(ctx->f);
	ctx->h.sigfigs = breaduint32(ctx->f);
	ctx->h.snaplen = breadint32(ctx->f);
	ctx->h.linktype = breadint32(ctx->f);

	if ((ctx->h.version_major != 2) || (ctx->h.version_minor != 4)) {
		return -1;
	}

	if (ctx->h.snaplen > PCAP_MAXFRAME) {
		return -1;
	}

	return 0;
}

int pcap_open_out(struct pcap_file *ctx, BINFILE * f) {
	return pcap_open_out_linktype(ctx, f, 1);
}

int pcap_open_out_linktype(struct pcap_file *ctx, BINFILE *f, int32_t linktype) {
	ctx->f = f;
	ctx->f->endian = ENDIAN_BIG;
	ctx->h.version_major = 2;
	ctx->h.version_minor = 4;
	ctx->h.thiszone = 0;
	ctx->h.sigfigs = 0;
	ctx->h.snaplen = PCAP_MAXFRAME;
	ctx->h.linktype = linktype;

	bwriteuint32(f, PCAP_MAGIC);
	bwriteuint16(f, ctx->h.version_major);
	bwriteuint16(f, ctx->h.version_minor);
	bwriteint32(f, ctx->h.thiszone);
	bwriteuint32(f, ctx->h.sigfigs);
	bwriteint32(f, ctx->h.snaplen);
	return bwriteint32(f, ctx->h.linktype);
}

void pcap_read_pkthdr(struct pcap_file *ctx, struct pcap_pkthdr *hdr) {
	 hdr->ts.tv_sec = breaduint32(ctx->f);
	 hdr->ts.tv_usec = breaduint32(ctx->f);
	 hdr->caplen = breaduint32(ctx->f);
	 hdr->len = breaduint32(ctx->f);
}

int pcap_write_pkthdr(struct pcap_file *ctx, struct pcap_pkthdr *hdr) {
	bwriteuint32(ctx->f, hdr->ts.tv_sec);
	bwriteuint32(ctx->f, hdr->ts.tv_usec);
	bwriteuint32(ctx->f, hdr->caplen);
	return bwriteuint32(ctx->f, hdr->len);
}
