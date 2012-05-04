ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS   = hdng unhex xor repr puniq pmerge p4split
dist_man1_MANS = man/hdng.mdoc man/unhex.mdoc man/xor.mdoc man/repr.mdoc \
				 man/puniq.mdoc man/pmerge.mdoc man/p4split.mdoc
dist_man7_MANS = man/netre-tools.mdoc
MANPAGES = $(dist_man1_MANS) $(dist_man7_MANS)
PDFPAGES = $(MANPAGES:man/%.mdoc=docs/%.pdf)

AM_CFLAGS =
AM_CPPFLAGS =
if IS_GCC
AM_CFLAGS += -std=c99
AM_CPPFLAGS += -D_GNU_SOURCE
if IS_DEBUG
AM_CFLAGS += -g -ggdb -O0
AM_CFLAGS += -Wno-long-long -Wall -W -Wnested-externs -Wformat=2
AM_CFLAGS += -Wstrict-prototypes
AM_CFLAGS += -Wwrite-strings -Wshadow -Wpointer-arith -Wsign-compare
AM_CFLAGS += -Wundef -Wbad-function-cast -Winline -Wcast-align -Wno-pointer-sign
endif
endif

if IS_DEBUG
AM_CPPFLAGS += -DDEBUGSTMTS
endif

noinst_LTLIBRARIES = libpcap.la
libpcap_la_SOURCES = src/pcap.c src/pcap.h

noinst_HEADERS     = src/netre.h

hdng_SOURCES       = src/hdng.c
unhex_SOURCES      = src/unhex.c
xor_SOURCES        = src/xor.c
repr_SOURCES       = src/repr.c
puniq_SOURCES      = src/puniq.c
puniq_LDADD        = libpcap.la
pmerge_SOURCES     = src/pmerge.c
pmerge_LDADD       = libpcap.la
p4split_SOURCES    = src/p4split.c
p4split_LDADD      = libpcap.la

EXTRA_DIST         = man docs

MAINTAINERCLEANFILES = Makefile.in aclocal.m4 build-aux/compile \
                       build-aux/config.guess build-aux/config.sub \
                       build-aux/depcomp build-aux/install-sh build-aux/missing \
                       config.h.in config.h.in~ configure build-aux/ltmain.sh \
                       m4/libtool.m4 m4/lt~obsolete.m4 m4/ltoptions.m4 \
                       m4/ltsugar.m4 m4/ltversion.m4 $(PDFPAGES)

docdir = $(datadir)/doc/@PACKAGE@
doc_DATA = README AUTHORS TODO $(PDFPAGES)

MDOC2PS  = groff -mdoc -t -Tps
PS2PDF   = ps2pdf -

docs:
	mkdir -p man
	mkdir -p docs
	for i in $(MANPAGES:man/%.mdoc=%); do \
		$(MDOC2PS) $(srcdir)/man/$$i.mdoc | $(PS2PDF) $(srcdir)/docs/$$i.pdf; \
	done

$(PDFPAGES): docs

maintainer-clean-local:
	rmdir build-aux
	rmdir m4
	-rmdir docs

dist-hook: docs

install-exec-hook:
	(cd $(DESTDIR)$(bindir);      \
		$(LN_S) xor rot;          \
		$(LN_S) xor rol;          \
		$(LN_S) xor caesar;       )

install-data-hook:
	(cd $(DESTDIR)$(man1dir);     \
		$(LN_S) xor.1 rot.1;      \
		$(LN_S) xor.1 rol.1;      \
		$(LN_S) xor.1 caesar.1;   )
	mv -f $(DESTDIR)$(docdir)/xor.pdf \
		$(DESTDIR)$(docdir)/transformer.pdf

uninstall-hook:
	rm $(DESTDIR)$(bindir)/{rot,rol,caesar}
	rm $(DESTDIR)$(man1dir)/{rot,rol,caesar}.1
	rm $(DESTDIR)$(docdir)/transformer.pdf
