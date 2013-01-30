#include <stdio.h>
#include <stdint.h>
#include <sysexits.h>
#include "pcap.h"

struct input_file {
	int active;
	struct pcap_file p;
	struct pcap_pkthdr next;
};

int
usage(int ret)
{
	fprintf(stderr, "Usage: pmerge FILE ...\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Merges pcap files, outputting time-ordered pcap stream\n");
	return ret;
}

int
read_next(struct input_file *file)
{
	if (!file->active)
		return -1;

	if (-1 == pcap_read_pkthdr(&file->p, &file->next)) {
		pcap_close(&file->p);
		file->active = 0;
		file->next.ts.tv_sec = 0xffffffff;
		file->next.ts.tv_usec = 0xffffffff;
		return -1;
	}

	return 0;
}

int
main(int argc, char *argv[])
{
	struct input_file files[argc - 1];
	int nfiles = 0;
	int nopen;
	int i;
	struct pcap_file out;

	if (1 == argc)
		return usage(0);

	/*
	 * Open input files 
	 */
	for (i = 0; i < argc - 1; i += 1) {
		char *fn = argv[i + 1];
		struct input_file *cur = &files[nfiles];
		FILE *f;

		if ('-' == fn[0])
			return usage(0);

		f = fopen(fn, "rb");
		if (NULL == f) {
			perror(fn);
			return EX_NOINPUT;
		}

		if (-1 == pcap_open_in(&cur->p, f)) {
			fprintf(stderr, "%s: unable to process\n", fn);
			return EX_IOERR;
		}
		cur->active = 1;

		if (0 == read_next(cur)) {
			nfiles += 1;
		}
	}

	if (-1 == pcap_open_out(&out, stdout)) {
		perror("writing header");
		return EX_IOERR;
	}

	nopen = nfiles;
	while (nopen) {
		struct input_file *cur = &files[0];
		char frame[MAXFRAME];
		size_t len;

		/*
		 * Find next oldest frame 
		 */
		for (i = 0; i < nfiles; i += 1) {
			if (files[i].active &&
			    ((files[i].next.ts.tv_sec < cur->next.ts.tv_sec) ||
			     ((files[i].next.ts.tv_sec == cur->next.ts.tv_sec) &&
			      (files[i].next.ts.tv_usec < cur->next.ts.tv_usec)))) {
				cur = &files[i];
			}
		}

		/*
		 * Make sure it'll fit 
		 */
		if (cur->next.caplen > sizeof(frame)) {
			fprintf(stderr, "error: huge frame (size %u)\n", (unsigned int) len);
			return EX_SOFTWARE;
		}

		/*
		 * Read it 
		 */
		len = fread(frame, 1, cur->next.caplen, cur->p.f);
		if (len < cur->next.caplen) {
			/*
			 * Short read 
			 */
			cur->next.caplen = len;
			pcap_close(&cur->p);
			cur->active = 0;
		}

		/*
		 * Write header + frame 
		 */
		if (len) {
			if (1 != fwrite(&cur->next, sizeof(cur->next), 1, stdout)) {
				perror("error");
				return EX_IOERR;
			}
			if (len != fwrite(frame, 1, len, stdout)) {
				perror("error");
				return EX_IOERR;
			}
		}

		if (-1 == read_next(cur)) {
			nopen -= 1;
		}
	}

	return 0;
}
