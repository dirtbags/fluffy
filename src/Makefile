CFLAGS = -Wall -Werror
TARGETS = pmerge puniq p4split hdng xor unhex repr

all: fake-configh $(TARGETS)

pmerge: pmerge.o pcap.o
puniq: puniq.o pcap.o
p4split: p4split.c pcap.o

fake-configh:
	echo "#define PACKAGE_VERSION \"0.1337\"" > config.h

clean:
	rm -f $(TARGETS) *.o
	rm -f config.h
