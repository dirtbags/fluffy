CFLAGS += -Wall -Werror -Wno-unused-variable -O2
TARGETS += pmerge puniq p4split pcat 
TARGETS += hd pyesc printfesc xor unhex 

all: $(TARGETS)

pmerge: pmerge.o pcap.o
puniq: puniq.o pcap.o
p4split: p4split.c pcap.o
pcat: pcat.c pcap.o stream.o

clean:
	rm -f $(TARGETS) *.o
