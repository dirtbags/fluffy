CFLAGS = -Wall -Werror
TARGETS = pmerge puniq p4split hd
TARGETS += pyesc printfesc

all: $(TARGETS)

pmerge: pmerge.o pcap.o
puniq: puniq.o pcap.o
p4split: p4split.c pcap.o

clean:
	rm -f $(TARGETS) *.o
