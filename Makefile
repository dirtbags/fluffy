DESTDIR ?= $(HOME)

CFLAGS = -Wall -Werror

TARGETS += pmerge 
TARGETS += puniq 
TARGETS += hd 
TARGETS += pyesc 
TARGETS += xor 
TARGETS += undec
TARGETS += unoct
TARGETS += pcat
TARGETS += slice
TARGETS += unhex
TARGETS += hex
TARGETS += bin
TARGETS += unbin
TARGETS += entropy
TARGETS += freq
TARGETS += histogram
TARGETS += printy
TARGETS += bubblebabble

SCRIPTS += octets

all: $(TARGETS)

install: $(TARGETS) $(SCRIPTS)
	install -d $(DESTDIR)$(PREFIX)/bin
	install $(TARGETS) $(DESTDIR)$(PREFIX)/bin
	install $(SCRIPTS) $(DESTDIR)$(PREFIX)/bin

uninstall:
	cd $(DESTDIR)$(PREFIX)/bin && rm -f $(TARGETS) $(SCRIPTS)

test: $(TARGETS) test.sh
	./test.sh

pmerge: pmerge.o pcap.o binio.o

puniq: puniq.o pcap.o binio.o

p4split: p4split.o pcap.o binio.o

pcat: pcat.o pcap.o binio.o
pcat.o: CFLAGS += -Wno-unused-variable

entropy: LDLIBS += -lm

clean:
	rm -f $(TARGETS) *.o
