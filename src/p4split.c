#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sysexits.h>
#include <strings.h>
#include "pcap.h"

/* Gaah so much crap */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/in.h>


int
usage(int ret)
{
  fprintf(stderr, "Usage: p4split CIDR\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Splits pcap on stdin into up to 256 files, based on CIDR.\n");
  return ret;
}


int
parse_cidr(char *s, uint32_t *addr, uint8_t *bits)
{
  char           *slash = index(s, '/');
  struct in_addr  inp;

  if (slash) {
    *slash = 0;
    *bits = atoi(slash + 1);
  } else {
    *bits = 0;
  }

  if (0 == inet_aton(s, &inp)) return -1;
  *addr = ntohl(inp.s_addr);

  return 0;
}
  

int
main(int argc, char *argv[])
{
  struct pcap_file p;
  struct pcap_file out[256];
  int              ok = 0;
  uint32_t         addr;
  uint32_t         mask;
  uint8_t          bits;
  uint8_t          shr;
  int              i;

  if (argc != 2) return usage(0);
  if (-1 == parse_cidr(argv[1], &addr, &bits)) return usage(0);
  if (bits > 24) return usage(0);
  if (bits % 8) {
    fprintf(stderr, "Warning: routing prefix is not a multiple of 8.\n");
  }

  mask = ~((1 << (32-bits)) - 1);
  addr &= mask;
  shr = (32-bits) - 8; 

  for (i = 0; i < 256; i += 1) {
    out[i].f = NULL;
  }

  if (-1 == pcap_open_in(&p, stdin)) return usage(0);

  while (1) {
    struct pcap_pkthdr hdr;
    uint8_t            octet;
    char               frame[MAXFRAME];

    ok = 1;
    if (-1 == pcap_read_pkthdr(&p, &hdr)) break;
    if (1 != fread(frame, hdr.caplen, 1, p.f)) break;


    {
      struct ether_header *eh = (struct ether_header *)frame;
      struct iphdr        *ih = (struct iphdr *)(frame + sizeof(struct ether_header));
      uint32_t             a;

      /* VLAN tag */
      if (ntohs(eh->ether_type) == 0x8100) {
        ih = (struct iphdr *)((char *)ih + 4);
      }

      a = ntohl(ih->saddr);
      if ((a & mask) != addr) {
        a = ntohl(ih->daddr);
        if ((a & mask) != addr) {
          fprintf(stderr, "Warning: dropping unmatched packet %08x -> %08x\n",
                  ntohl(ih->saddr), ntohl(ih->daddr));
          continue;
        }
      }
      octet = (a & ~mask) >> shr;
    }

    ok = 0;
    if (! out[octet].f) {
      char fn[9];
      FILE *f;

      sprintf(fn, "%d.pcap", octet);

      if (NULL == (f = fopen(fn, "wb"))) break;
      if (-1 == pcap_open_out(&out[octet], f)) break;
    }

    if (-1 == pcap_write_pkthdr(&out[octet], &hdr)) break;
    if (1 != fwrite(frame, hdr.caplen, 1, out[octet].f)) break;
  }

  if (! ok) {
    perror("Error");
    return 1;
  }

  for (i = 0; i < 256; i += 1) {
    if (out[i].f) {
      pcap_close(&p);
    }
  }

  return 0;
}
