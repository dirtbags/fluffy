CFLAGS = -Wall -Werror
TARGETS = pmerge puniq p4split pcat 
TARGETS += hd pyesc printfesc xor unhex 

all: $(TARGETS)

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
