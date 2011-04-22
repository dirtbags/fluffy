CFLAGS = -Wall -Werror
TARGETS = pmerge puniq

all: $(TARGETS)

pmerge: pmerge.o pcap.o
puniq: puniq.o pcap.o

clean:
	rm -f $(TARGETS)
