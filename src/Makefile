#!/usr/bin/make -f

prefix = /usr/local
bindir = $(prefix)/bin
man1dir = $(prefix)/share/man/man1
man7dir = $(prefix)/share/man/man7
docdir = $(prefix)/share/doc/netre-tools

mansrc = ../man

TARGETS = pmerge puniq p4split hdng xor unhex repr

all: config.h $(TARGETS)

pmerge: pmerge.o pcap.o
puniq: puniq.o pcap.o
p4split: p4split.c pcap.o

CC = gcc
CFLAGS = -g -O2 -std=c99
CPPFLAGS = -D_GNU_SOURCE

# debug:
# CFLAGS += -g -ggdb -O0
# CFLAGS += -Wno-long-long -Wall -W -Wnested-externs -Wformat=2
# CFLAGS += -Wstrict-prototypes
# CFLAGS += -Wwrite-strings -Wshadow -Wpointer-arith -Wsign-compare
# CFLAGS += -Wundef -Wbad-function-cast -Winline -Wcast-align -Wno-pointer-sign
# CPPFLAGS += -DDEBUGSTMTS
# INSTALL = install

INSTALL = install -s
INSTALL_DATA = install
LN_S = ln -s

install:
	for target in $(TARGETS); do \
		$(INSTALL) -D -m 755 $$target $(DESTDIR)$(bindir)/$$target; \
		$(INSTALL_DATA) -D -m 644 $(mansrc)/$$target.mdoc $(DESTDIR)$(man1dir)/$$target.1; \
	done
	$(INSTALL_DATA) -D -m 644 $(mansrc)/netre-tools.mdoc $(DESTDIR)$(man7dir)/netre-tools.7
	(cd $(DESTDIR)$(bindir); \
		$(LN_S) xor rot; \
		$(LN_S) xor rol; \
		$(LN_S) xor caesar; )
	(cd $(DESTDIR)$(man1dir); \
		$(LN_S) xor.1 rot.1; \
		$(LN_S) xor.1 rol.1; \
		$(LN_S) xor.1 caesar.1; )

config.h:
	echo "#define PACKAGE_VERSION \"0.1337\"" > config.h

clean:
	rm -f $(TARGETS) *.o
	rm -f config.h
