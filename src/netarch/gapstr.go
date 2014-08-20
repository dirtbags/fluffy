package netarch

import (
	"bytes"
)

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}

func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}

type chunk interface{}

func chunkLen(c chunk) int {
	switch c.(type) {
	case int:
		return c.(int)
	case string:
		return len(c.(string))
	}
	return 0
}

func chunkSlice(c chunk, beg, end int) chunk {
	switch c.(type) {
	case int:
		return end - beg
	case string:
		return c.(string)[beg:end]
	}
	return 0
}

// The type of a function to be passed to Map
type Processor func(int, string) string

type Gapstr struct {
	chunks []chunk
}

func (g *Gapstr) Length() int {
	ret := 0

	for _, c := range g.chunks {
		ret += chunkLen(c)
	}
	return ret
}

func (g *Gapstr) HasGaps() bool {
	for _, c := range g.chunks {
		switch c.(type) {
		case int:
			return true
		}
	}
	return false
}

func (g *Gapstr) Copy() *Gapstr {
	chunks := make([]chunk, len(g.chunks))
	copy(chunks, g.chunks)
	return &Gapstr{chunks}
}

func (g *Gapstr) AppendString(s string) *Gapstr {
	return &Gapstr{append(g.chunks, s)}
}

func (g *Gapstr) AppendGap(len int) *Gapstr {
	return &Gapstr{append(g.chunks, len)}
}

func (g *Gapstr) String() string {
	var b bytes.Buffer
	for _, c := range g.chunks {
		switch c.(type) {
		case int:
			b.Write(bytes.Repeat([]byte{0}, c.(int)))
		case string:
			b.WriteString(c.(string))
		}
	}
	return b.String()
}

// Apply proc to everything in g
func (g *Gapstr) Map(proc Processor) *Gapstr {
	chunks := make([]chunk, 0, len(g.chunks))
	pos := 0

	for _, c := range g.chunks {
		switch c.(type) {
		case int:
			chunks = append(chunks, c)
		case string:
			chunks = append(chunks, proc(pos, c.(string)))
		}
		pos += chunkLen(c)
	}

	return &Gapstr{chunks}
}

func (g *Gapstr) Xor(mask []byte) *Gapstr {
	xorproc := func(pos int, s string) string {
		var b bytes.Buffer

		for i := 0; i < len(s); i += 1 {
			p := (pos + i) % len(mask)
			b.WriteByte(s[i] ^ mask[p])
		}
		return b.String()
	}
	return g.Map(xorproc)
}

func (g *Gapstr) Slice(beg, end int) *Gapstr {
	pos := 0
	chunks := make([]chunk, 0)

	for _, c := range g.chunks {
		cl := chunkLen(c)
		
		if (pos + cl > beg) && (pos <= end) {
			cb := max(beg - pos, 0)
			ce := min(end - pos, cl)
		
			chunks = append(chunks, chunkSlice(c, cb, ce))
		}
		
		pos += cl
	}
	
	return &Gapstr{chunks}
}

type GapstrReader struct {
	g *Gapstr
	pos int
	GapRead bool
}

func NewGapstrReader(g *Gapstr) (*GapstrReader) {
	return &GapstrReader{g, 0, false}
}

func (gr *GapstrReader) Read(p []byte) (n int, err error) {
	gs := gr.g.Slice(gr.pos, gr.pos + len(p))
	if gs.HasGaps() {
		gr.GapRead = true
	}
	
	s := []byte(gs.String())
	nread := copy(p, s)
	gr.pos += nread
	return nread, nil
}