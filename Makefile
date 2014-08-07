CFLAGS = -Wall -Werror
TARGETS += puniq
TARGETS += hd

GOPATH = $(CURDIR)
export GOPATH

all: $(TARGETS)

%: src/%.go
	go build $@

%: %.go
	go build $<
	
install: $(TARGETS)
	install -d $(DESTDIR)$(PREFIX)
	install $(TARGETS) $(DESTDIR)$(PREFIX)/bin

pmerge: pmerge.o pcap.o

puniq: puniq.o pcap.o

p4split: p4split.o pcap.o

pcat: pcat.o pcap.o stream.o
pcat.o: CFLAGS += -Wno-unused-variable

clean:
	rm -f $(TARGETS) *.o
