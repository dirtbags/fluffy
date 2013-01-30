#include <stdio.h>
#include <sysexits.h>
#include <string.h>
#include "pcap.h"

int
main(int argc, char *argv[])
{
	int i;
	struct pcap_pkthdr hdr[2];
	char frame[2][MAXFRAME];
	int cur = 0;
	struct pcap_file out;

	memset(&hdr, 0, sizeof(hdr));
	memset(frame, 0, 2 * MAXFRAME);

	if (-1 == pcap_open_out(&out, stdout)) {
		perror("writing header");
		return EX_IOERR;
	}

	i = 1;
	do {
		char *fn = argv[i];
		FILE *f;
		struct pcap_file p;

		if ((!fn) || (0 == strcmp("-", fn))) {
			f = stdin;
		} else {
			f = fopen(fn, "rb");
			if (NULL == f) {
				perror(fn);
				return EX_IOERR;
			}
		}

		if (-1 == pcap_open_in(&p, f)) {
			fprintf(stderr, "%s: unable to process\n", fn);
			return EX_IOERR;
		}

		while (1) {
			if (-1 == pcap_read_pkthdr(&p, &hdr[cur]))
				break;
			if (1 != fread(frame[cur], hdr[cur].caplen, 1, p.f))
				break;

			if ((0 == memcmp(&hdr[0], &hdr[1], sizeof(hdr[0]))) && (0 == memcmp(frame[0], frame[1], hdr[cur].caplen))) {
				/*
				 * Skip this duplicate 
				 */
			} else {
				if (-1 == pcap_write_pkthdr(&out, &hdr[cur]))
					break;
				if (1 != fwrite(frame[cur], hdr[cur].caplen, 1, out.f))
					break;
			}
			cur = (1 - cur);
		}
		pcap_close(&p);

		i += 1;
	} while (i < argc);

	return 0;
}
