TARGETS += netarch
TARGETS += puniq

GOPATH = $(CURDIR)
export GOPATH

all:
	go build $(TARGETS)

test:
	go test $(TARGETS)

clean:
	go clean $(TARGETS)
	rm -f $(TARGETS)
