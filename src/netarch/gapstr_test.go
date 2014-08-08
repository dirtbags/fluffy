package netarch

import (
	"testing"
)

func AssertEqual(t *testing.T, a, b interface{}) {
	if a != b {
		t.Errorf("Not equal: %#v, %#v", a, b)
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
