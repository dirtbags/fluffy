package netarch

import (
	"bytes"
)

type chunk interface{}

type Gapstr struct {
	chunks []chunk
}

func GapstrOfString(s string) *Gapstr {
	return &Gapstr{[]chunk{s}}
}

func (g *Gapstr) appendChunk(c chunk) *Gapstr {
	return &Gapstr{append(g.chunks, c)}
}

func (g *Gapstr) AppendString(s string) *Gapstr {
	return g.appendChunk(s)
}

func (g *Gapstr) AppendGap(len int) *Gapstr {
	return g.appendChunk(len)
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