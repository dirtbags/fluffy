package pcap

import (
	"bytes"
	"io"
	"math/rand"
	"os"
	"testing"
	"time"
)

func randomString(prng *rand.Rand, length int) string {
	var b bytes.Buffer
	
	for i := 0; i < length; i += 1 {
		b.WriteByte(byte(prng.Int() % 256))
	}
	return b.String()
}

// Just read in a file, shouldn't be any errors.
func TestReader(t *testing.T) {
	f, err := os.Open("example.pcap")
	if err != nil {
		t.Fatal(err)
	}
	defer f.Close()
	
	pin, err := NewReader(f)
	if err != nil {
		t.Fatal(err)
	}
	
	for {
		frame, err := pin.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			t.Fatal(err)
		}
		
		if len(frame.Payload) > 2000 {
			t.Error("Unusually large payload")
		}
	}
}

func TestReadWrite(t *testing.T) {
	prng := rand.New(rand.NewSource(time.Now().Unix()))
	frames := make([]Frame, 20)
	
	outf := new(bytes.Buffer)
	pout, err := NewWriter(outf)
	if err != nil {
		t.Fatal(err)
	}
	
	for _, f := range frames {
		f.Header.Sec = prng.Uint32()
		f.Header.Usec = prng.Uint32()
		f.Header.Caplen = prng.Uint32()
		f.Header.Framelen = prng.Uint32()
		f.Payload = randomString(prng, prng.Intn(400))
		
		pout.WriteFrame(f)
	}
	
	pin, err := NewReader(outf)
	if err != nil {
		t.Fatal(err)
	}
	for _, f := range frames {
		ff, err := pin.Read()
		if err != nil {
			t.Fatal(err)
		}
		
		if *ff != f {
			t.Fatal(ff, f)
		}
	}		
}
