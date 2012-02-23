ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = man

noinst_LTLIBRARIES = libpcap.la
libpcap_la_SOURCES = src/pcap.c src/pcap.h

noinst_HEADERS = src/netre.h

bin_PROGRAMS = hdng unhex xor repr puniq pmerge p4split
hdng_SOURCES = src/hdng.c
unhex_SOURCES = src/unhex.c
xor_SOURCES = src/xor.c
repr_SOURCES = src/repr.c
puniq_SOURCES = src/puniq.c
puniq_LDADD = libpcap.la
pmerge_SOURCES = src/pmerge.c
pmerge_LDADD = libpcap.la
p4split_SOURCES = src/p4split.c
p4split_LDADD = libpcap.la

man_MANS = man/hdng.1 man/unhex.1 man/xor.1 man/repr.1 man/puniq.1 \
		   man/pmerge.1 man/p4split.1

docdir = $(datadir)/doc/@PACKAGE@
doc_DATA = README AUTHORS TODO

MAINTAINERCLEANFILES = Makefile.in aclocal.m4 build-aux/compile \
			build-aux/config.guess build-aux/config.sub build-aux/depcomp \
			build-aux/install-sh build-aux/missing config.h.in \
			config.h.in~ configure build-aux/ltmain.sh m4/libtool.m4 \
			m4/lt~obsolete.m4 m4/ltoptions.m4 m4/ltsugar.m4 m4/ltversion.m4

maintainer-clean-local:
	rmdir build-aux
	rmdir m4
