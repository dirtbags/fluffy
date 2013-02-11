#ifndef __PCAP_H__
#define __PCAP_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define MAGIC 0xa1b2c3d4
#define MAXFRAME 9000

/* Described at http://www.tcpdump.org/linktypes.html */
#define LINKTYPE_ETHERNET 1
#define LINKTYPE_RAW 101

struct pcap_file {
	FILE *f;
	uint32_t linktype;
	bool swap;
};

struct pcap_file_header {
	uint32_t magic;
	uint16_t version_major;
	uint16_t version_minor;
	int32_t thiszone;	/* gmt to local correction */
	uint32_t sigfigs;	/* accuracy of timestamps */
	int32_t snaplen;	/* max length saved portion of each pkt */
	int32_t linktype;	/* data link type (LINKTYPE_*) */
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

#define bswap32(i) (((i & 0xff000000) >> 030) | \
                    ((i & 0x00ff0000) >> 010) | \
                    ((i & 0x0000ff00) << 010) | \
                    ((i & 0x000000ff) << 030))
#define bswap16(i) (((i & 0xff00) >> 010) | \
                    ((i & 0x00ff) << 010))


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

int pcap_open_in(struct pcap_file *ctx, FILE * f);
int pcap_open_out(struct pcap_file *ctx, FILE * f);
int pcap_read_pkthdr(struct pcap_file *ctx, struct pcap_pkthdr *hdr);
int pcap_write_pkthdr(struct pcap_file *ctx, struct pcap_pkthdr *hdr);
void pcap_close(struct pcap_file *ctx);

#endif				/* __PCAP_H__ */
