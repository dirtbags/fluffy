/*
 * pcat.c -- pcap to text
 *
 * Reads pcap on stdin, dumps text representation to stdout.
 * This discards or ignores some parts of packets. If fidelity is a concern,
 * deal directly with the original pcap.
 *
 * Output lines are of the form:
 *
 * 	PROTO SRC DST OPTS PAYLOAD
 *
 
 */

#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include "pcap.h"
#include "binio.h"

#define IPPROTO_TCP 6
#define IPPROTO_UDP 17
#define IPPROTO_ICMP 1

#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20

void ip4_addr(char *addr_s, uint32_t addr) {
	snprintf(addr_s, 16, "%u.%u.%u.%u", (addr >> 24) & 0xff, (addr >> 16) & 0xff, (addr >> 8) & 0xff, (addr >> 0) & 0xff);
}

void print_nybble(uint8_t nybble) {
	if (nybble < 10) {
		putchar(nybble + '0');
	} else {
		putchar(nybble - 10 + 'a');
	}
}

/* About 3x faster than printf("%02x", octet); */
void printx(uint8_t octet) {
	print_nybble(octet >> 4);
	print_nybble(octet & 0xf);
}

void print_payload(BINFILE *r) {
	while (!feof(r->f)) {
		printx(breaduint8(r));
	}
}

void process_tcp(BINFILE *r, char *saddr_s, char *daddr_s) {
	uint16_t sport = breaduint16(r);
	uint16_t dport = breaduint16(r);
	uint32_t seq = breaduint32(r);
	uint32_t ack = breaduint32(r);
	uint8_t off = breaduint8(r);
	uint8_t hlen = (off >> 4) * 4;
	uint8_t flags = breaduint8(r);
	uint16_t window = breaduint16(r);
	uint16_t chksum = breaduint16(r);
	uint16_t urgent = breaduint16(r);

	if (hlen < 20) {
		printf("!");
	}

	printf("TCP\t%s,%u\t%s,%u\t%u,%u,%d", saddr_s, sport, daddr_s, dport, seq, ack, flags);
}

void process_udp(BINFILE *r, char *saddr_s, char *daddr_s) {
	uint16_t sport = breaduint16(r);
	uint16_t dport = breaduint16(r);
	uint16_t len = breaduint16(r);
	uint16_t chksum = breaduint16(r);

	printf("UDP\t%s,%u\t%s,%u\t0", saddr_s, sport, daddr_s, dport);
}

void process_icmp(BINFILE *r, char *saddr_s, char *daddr_s) {
	uint8_t type = breaduint8(r);
	uint8_t code = breaduint8(r);
	uint16_t checksum = breaduint16(r);
	
	printf("ICMP\t%s\t%s\t%d,%d", saddr_s, daddr_s, type, code);
}

void process_ip4(BINFILE *r) {
	uint8_t vhl = breaduint8(r);
	uint8_t ihl = (vhl & 0x0f) * 4;
	uint8_t tos = breaduint8(r);
	uint16_t length = breaduint16(r);
	uint16_t id = breaduint16(r);
	uint16_t off = breaduint16(r);
	uint8_t ttl = breaduint8(r);
	uint8_t proto = breaduint8(r);
	uint16_t chksum = breaduint16(r);
	uint32_t saddr = breaduint32(r);
	uint32_t daddr = breaduint32(r);

	char saddr_s[20];
	char daddr_s[20];

	ip4_addr(saddr_s, saddr);
	ip4_addr(daddr_s, daddr);

	// Ignore options
	breadskip(r, 20 - ihl);

	printf("IP4,");
	switch (proto) {
		case IPPROTO_TCP:
			process_tcp(r, saddr_s, daddr_s);
			break;
		case IPPROTO_UDP:
			process_udp(r, saddr_s, daddr_s);
			break;
		case IPPROTO_ICMP:
			process_icmp(r, saddr_s, daddr_s);
			break;
		default:
			printf("P%d\t%s\t%s", proto, saddr_s, daddr_s);
			break;
	}
}


void print_ethernet(BINFILE *r) {
	uint8_t saddr[6] = {0};
	uint8_t daddr[6] = {0};
	uint16_t ethertype;

	fread(saddr, 6, 1, r->f);
	fread(daddr, 6, 1, r->f);
	ethertype = breaduint16(r);

	if (ethertype == 0x8100) {
		// VLAN 
		breaduint16(r);
		ethertype = breaduint16(r);
	}

	switch (ethertype) {
		case 0x0800:	// IPv4
			process_ip4(r);
			break;
		default:
			printf("E%d\t.\t.\t.", ethertype);
			break;
	}
	printf("\t");
	print_payload(r);
}

void print_frame(struct pcap_file *p, struct pcap_pkthdr *hdr, BINFILE *r) {
	printf("%u.%u\t", hdr->ts.tv_sec, hdr->ts.tv_usec);
	switch (p->h.linktype) {
		case LINKTYPE_ETHERNET:
			print_ethernet(r);
			break;
		case LINKTYPE_RAW:
			process_ip4(r);
			break;
	}
	printf("\n");
}

int main(int argc, char *argv[]) {
	BINFILE bf = {stdin, ENDIAN_NETWORK};
	struct pcap_file p;
	struct pcap_pkthdr hdr;
	char buf[PCAP_MAXFRAME];
        
	if (-1 == pcap_open_in(&p, &bf)) {
		fprintf(stderr, "pcap_open_in: error\n");
		return -1;
	}

	while (!feof(stdin)) {
		pcap_read_pkthdr(&p, &hdr);

		if (hdr.caplen > PCAP_MAXFRAME) {
			fprintf(stderr, "Frame too large: %d\n", hdr.caplen);
			return -1;
		}

		if (1 != fread(buf, hdr.caplen, 1, bf.f)) {
			if (feof(bf.f)) {
				break;
			}
			perror("fread");
			return -1;
		}

		FILE *ff = fmemopen(buf, hdr.caplen, "r");
		if (!ff) {
			perror("fmemopen");
			return -1;
		}

		BINFILE bff = {ff, ENDIAN_NETWORK};
		print_frame(&p, &hdr, &bff);

		fclose(ff);
	}

	return 0;
}
