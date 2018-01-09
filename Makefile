CFLAGS = -Wall -Werror

TARGETS += pmerge 
TARGETS += puniq 
TARGETS += hd 
TARGETS += pyesc 
TARGETS += xor 
TARGETS += unhex
TARGETS += pcat
TARGETS += slice
TARGETS += hex
TARGETS += entropy

SCRIPTS += octets

all: $(TARGETS)

install: $(TARGETS) $(SCRIPTS)
	install -d $(DESTDIR)$(PREFIX)/bin
	install $(TARGETS) $(DESTDIR)$(PREFIX)/bin
	install $(SCRIPTS) $(DESTDIR)$(PREFIX)/bin

pmerge: pmerge.o pcap.o

puniq: puniq.o pcap.o

p4split: p4split.o pcap.o

pcat: pcat.o pcap.o stream.o
pcat.o: CFLAGS += -Wno-unused-variable

clean:
	rm -f $(TARGETS) *.o
