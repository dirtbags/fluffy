CFLAGS = -Wall -Werror
TARGETS = pmerge puniq p4split

all: $(TARGETS)

pmerge: pmerge.o pcap.o
puniq: puniq.o pcap.o
p4split: p4split.c pcap.o

clean:
	rm -f $(TARGETS) *.o
