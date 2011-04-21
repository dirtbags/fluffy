#include <stdio.h>
#include <sysexits.h>
#include "pcap.h"

int
pcap_read_header(FILE *f)
{
  struct pcap_file_header h;
  int                     swap;

  if (1 != fread(&h, sizeof(h), 1, f)) {
    h.magic = 0;
  }

  if (MAGIC == h.magic) {
    swap = 0;
  } else if (bswap32(MAGIC) == h.magic) {
    swap = 1;
  } else {
    return -1;
  }
  if ((h.version_major != 2) || (h.version_minor != 4)) return -1;

  if (swap) h.snaplen = bswap32(h.snaplen);
  if (h.snaplen > MAXFRAME) return -1;

  return swap;
}

int
pcap_write_header(FILE *f)
{
  struct pcap_file_header h = { MAGIC, 2, 4, 0, 0, MAXFRAME, 1 };

  if (1 != fwrite(&h, sizeof(h), 1, stdout)) {
    return -1;
  }

  return 0;
}

int
pcap_open_in(struct pcap_file *ctx, FILE *f)
{
  int ret;

  ret = pcap_read_header(f);
  if (-1 == ret) return -1;

  ctx->swap = ret;
  ctx->f = f;
  return 0;
}

int
pcap_read_pkthdr(struct pcap_file *ctx, struct pcap_pkthdr *hdr)
{
  if (1 != fread(hdr, sizeof(*hdr), 1, ctx->f)) {
    return -1;
  }

  if (ctx->swap) {
    hdr->ts.tv_sec  = bswap32(hdr->ts.tv_sec);
    hdr->ts.tv_usec = bswap32(hdr->ts.tv_usec);
    hdr->caplen     = bswap32(hdr->caplen);
    hdr->len        = bswap32(hdr->len);
  }

  return 0;
}

void
pcap_close(struct pcap_file *ctx)
{
  fclose(ctx->f);
}
