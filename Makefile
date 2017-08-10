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

all: $(TARGETS)

install: $(TARGETS)
	install -d $(DESTDIR)$(PREFIX)/bin
	install $(TARGETS) $(DESTDIR)$(PREFIX)/bin

pmerge: pmerge.o pcap.o

puniq: puniq.o pcap.o

p4split: p4split.o pcap.o

pcat: pcat.o pcap.o stream.o
pcat.o: CFLAGS += -Wno-unused-variable

clean:
	rm -f $(TARGETS) *.o
