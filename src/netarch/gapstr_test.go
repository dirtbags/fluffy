package netarch

import (
	"fmt"
	"runtime"
	"testing"
)

func DUMP(a ...interface{}) {
	_, file, line, _ := runtime.Caller(1)
	fmt.Printf("%s:%d ", file, line)
	fmt.Println(a...)
}
    
func AssertEqual(t *testing.T, a, b interface{}) {
	_, file, line, _ := runtime.Caller(1)
	if a != b {
		t.Errorf("%s:%d Not equal: %#v, %#v", file, line, a, b)
	}
}

func TestChunk(t *testing.T) {
	c := chunk("hello")
	AssertEqual(t, chunkLen(c), 5)
	AssertEqual(t, chunkSlice(c, 0, 3), "hel")
	AssertEqual(t, chunkSlice(c, 3, 5), "lo")
	
	c = chunk(5)
	AssertEqual(t, chunkLen(c), 5)
	AssertEqual(t, chunkSlice(c, 0, 3), 3)
	AssertEqual(t, chunkSlice(c, 3, 5), 2)
}

func TestGapstr(t *testing.T) {
	g := new(Gapstr)
	g = g.AppendString("hello")
	g = g.AppendGap(10)
	g = g.AppendString(" world")
	
	gs := g.String()
	AssertEqual(t, gs,  "hello\000\000\000\000\000\000\000\000\000\000 world")
	
	for i := 0; i <= 20; i += 4 {
		j := i+4
		if j > len(gs) {
			j = len(gs)
		}
		AssertEqual(t, g.Slice(i, i+4).String(), gs[i:j])
	}
	
	AssertEqual(t, g.Slice(0, 12).String(), gs[0:12])
}

func TestGapstrReader(t *testing.T) {
	g := new(Gapstr)
	g = g.AppendString("\xff\xffaoeu")
	g = g.AppendGap(8)
	g = g.AppendString("\xff\xff\xff")
	
	gs := g.String()
	
	gr := NewGapstrReader(g)
	
	b := make([]byte, 4)
	for i := 0; i < 4; i += 1 {
		beg := i*4
		end := min((i+1) * 4, len(gs))
		truth := gs[beg:end]
		nread, err := gr.Read(b)
		
		DUMP(gr.GapRead())
		AssertEqual(t, nread, len(truth))
		AssertEqual(t, err, nil)
		AssertEqual(t, string(b[:nread]), truth)
	}
}