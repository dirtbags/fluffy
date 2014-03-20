CFLAGS = -Wall -Werror
TARGETS += pmerge 
TARGETS += puniq 
TARGETS += p4split 
TARGETS += hd 
TARGETS += pyesc 
TARGETS += printfesc 
TARGETS += xor 
TARGETS += unhex

all: $(TARGETS)

nstall: $(TARGETS)
	install -d $(DESTDIR)$(PREFIX)
	install $(TARGETS) $(DESTDIR)$(PREFIX)/bin

pmerge: pmerge.o pcap.o

puniq: puniq.o pcap.o

p4split: p4split.o pcap.o

pcat: pcat.o pcap.o stream.o
pcat.o: CFLAGS += -Wno-unused-variable

clean:
	rm -f $(TARGETS) *.o
