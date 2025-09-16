#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "binio.h"

#define PCAP_MAGIC 0xa1b2c3d4
#define PCAP_CIGAM 0xd4c3b2a1
#define PCAP_MAXFRAME 262144

/*
 * Described at http://www.tcpdump.org/linktypes.html
 */
#define LINKTYPE_ETHERNET 1
#define LINKTYPE_RAW 101

struct pcap_file_header {
	uint16_t version_major;
	uint16_t version_minor;
	int32_t thiszone;	/* gmt to local correction */
	uint32_t sigfigs;	/* accuracy of timestamps */
	int32_t snaplen;	/* max length saved portion of each pkt */
	int32_t linktype;	/* data link type (LINKTYPE_*) */
};

struct pcap_file {
	BINFILE *f;
	struct pcap_file_header h;
};

struct pcap_pkthdr {
	struct pcap_timeval {
		uint32_t tv_sec;
		uint32_t tv_usec;
	} ts;			/* time stamp */
	uint32_t caplen;	/* length of portion present */
	uint32_t len;		/* length this packet (off wire) */
};

#ifndef max
#define max(a, b) ((a)>(b)?(a):(b))
#endif

#ifndef min
#define min(a, b) ((a)<(b)?(a):(b))
#endif

/*
 * Debugging help
 */
#define DUMPf(fmt, args...) fprintf(stderr, "%s:%s:%d " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ##args)
#define DUMP() DUMPf("")
#define DUMP_d(v) DUMPf("%s = %d", #v, v)
#define DUMP_u(v) DUMPf("%s = %u", #v, v)
#define DUMP_x(v) DUMPf("%s = 0x%x", #v, v)
#define DUMP_s(v) DUMPf("%s = %s", #v, v)
#define DUMP_c(v) DUMPf("%s = %c", #v, v)
#define DUMP_p(v) DUMPf("%s = %p", #v, v)

int pcap_open_in(struct pcap_file *ctx, BINFILE * f);
int pcap_open_out(struct pcap_file *ctx, BINFILE * f);
int pcap_open_out_linktype(struct pcap_file *ctx, BINFILE * f, int32_t linktype);
void pcap_read_pkthdr(struct pcap_file *ctx, struct pcap_pkthdr *hdr);
int pcap_write_pkthdr(struct pcap_file *ctx, struct pcap_pkthdr *hdr);
