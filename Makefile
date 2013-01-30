CFLAGS = -Wall -Werror
TARGETS = pmerge puniq p4split hd
TARGETS += pyesc printfesc pcat

all: $(TARGETS)

pmerge: pmerge.o pcap.o
puniq: puniq.o pcap.o
p4split: p4split.c pcap.o
pcat: pcat.c pcap.o stream.o

clean:
	rm -f $(TARGETS) *.o
