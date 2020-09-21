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

#include <stdio.h>
#include <stdint.h>
#include "pcap.h"
#include "stream.h"

#define IPPROTO_TCP 6
#define IPPROTO_UDP 17
#define IPPROTO_ICMP 1

#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20

void
ip4_addr(char *addr_s, uint32_t addr)
{
	snprintf(addr_s, 16, "%u.%u.%u.%u", (addr >> 24) & 0xff, (addr >> 16) & 0xff, (addr >> 8) & 0xff, (addr >> 0) & 0xff);
}

void
print_nybble(uint8_t nybble)
{
	if (nybble < 10) {
		putchar(nybble + '0');
	} else {
		putchar(nybble - 10 + 'a');
	}
}

/* About 3x faster than printf("%02x", octet); */
void
printx(uint8_t octet)
{
	print_nybble(octet >> 4);
	print_nybble(octet & 0xf);
}

void
print_payload(struct stream *s)
{
	while (s->len) {
		printx(read_uint8(s));
	}
}

void
process_tcp(struct stream *s, char *saddr_s, char *daddr_s)
{
	uint16_t sport = read_uint16(s);
	uint16_t dport = read_uint16(s);
	uint32_t seq = read_uint32(s);
	uint32_t ack = read_uint32(s);
	uint8_t off = read_uint8(s);
	uint8_t hlen = (off >> 4) * 4;
	uint8_t flags = read_uint8(s);
	uint16_t window = read_uint16(s);
	uint16_t chksum = read_uint16(s);
	uint16_t urgent = read_uint16(s);

	if (hlen < 20) {
		printf("!");
	}

	printf("TCP\t%s,%u\t%s,%u\t%u,%u,%d\t", saddr_s, sport, daddr_s, dport, seq, ack, flags);
}

void
process_udp(struct stream *s, char *saddr_s, char *daddr_s)
{
	uint16_t sport = read_uint16(s);
	uint16_t dport = read_uint16(s);
	uint16_t len = read_uint16(s);
	uint16_t chksum = read_uint16(s);

	printf("UDP\t%s,%u\t%s,%u\t0\t", saddr_s, sport, daddr_s, dport);
}

void
process_icmp(struct stream *s, char *saddr_s, char *daddr_s)
{
	uint8_t type = read_uint8(s);
	uint8_t code = read_uint8(s);
	uint16_t checksum = read_uint16(s);
	
	printf("ICMP\t%s\t%s\t%d,%d\t", saddr_s, daddr_s, type, code);
}

void
process_ip4(struct stream *s)
{
	uint8_t vhl = read_uint8(s);
	uint8_t ihl = (vhl & 0x0f) * 4;
	uint8_t tos = read_uint8(s);
	uint16_t length = read_uint16(s);
	uint16_t id = read_uint16(s);
	uint16_t off = read_uint16(s);
	uint8_t ttl = read_uint8(s);
	uint8_t proto = read_uint8(s);
	uint16_t chksum = read_uint16(s);
	uint32_t saddr = read_uint32(s);
	uint32_t daddr = read_uint32(s);

	char saddr_s[20];
	char daddr_s[20];

	ip4_addr(saddr_s, saddr);
	ip4_addr(daddr_s, daddr);

	// Ignore options
	sskip(s, 20 - ihl);

	// Force stream length to IP payload length
	s->len = length - ihl;

	switch (proto) {
		case IPPROTO_TCP:
			process_tcp(s, saddr_s, daddr_s);
			break;
		case IPPROTO_UDP:
			process_udp(s, saddr_s, daddr_s);
			break;
		case IPPROTO_ICMP:
			process_icmp(s, saddr_s, daddr_s);
			break;
		default:
			printf("P%d\t%s\t%s\t", proto, saddr_s, daddr_s);
			break;
	}

	print_payload(s);
}


void
print_ethernet(struct stream *s)
{
	uint8_t saddr[6];
	uint8_t daddr[6];
	uint16_t ethertype;

	sread(s, &saddr, sizeof(saddr));
	sread(s, &daddr, sizeof(daddr));
	ethertype = read_uint16(s);

	if (ethertype == 0x8100) {
		// VLAN 
		read_uint16(s);
		ethertype = read_uint16(s);
	}

	switch (ethertype) {
		case 0x0800:	// IPv4
			process_ip4(s);
			break;
	}
}

void
print_frame(struct pcap_file *p, struct pcap_pkthdr *hdr, char const *frame)
{
	struct stream streambuf;
	struct stream *s = &streambuf;

	sinit(s, frame, hdr->caplen, ENDIAN_NETWORK); // pcap.c always outputs network byte order
	printf("%u.%u\t", hdr->ts.tv_sec, hdr->ts.tv_usec);
	switch (p->linktype) {
		case LINKTYPE_ETHERNET:
			print_ethernet(s);
			break;
		case LINKTYPE_RAW:
			process_ip4(s);
			break;
	}
	printf("\n");
}

void
pcat(FILE * f)
{
	struct pcap_file p;
	struct pcap_pkthdr hdr;
	char frame[MAXFRAME];

	if (-1 == pcap_open_in(&p, f))
		return;

	for (;;) {

		if (-1 == pcap_read_pkthdr(&p, &hdr)) {
			break;
		}

		if (1 != fread(frame, hdr.caplen, 1, f)) {
			break;
		}

		print_frame(&p, &hdr, frame);
	}
}


int
main(int argc, char *argv[])
{
	pcat(stdin);

	return 0;
}
