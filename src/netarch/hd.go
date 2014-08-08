package netarch

import (
	"fmt"
	"io"
	"os"
)

const charset = "" +
	"·☺☻♥♦♣♠•◘○◙♂♀♪♫☼" +
	"►◄↕‼¶§▬↨↑↓→←∟↔▲▼" +
	" !\"#$%&'()*+,-./" +
	"0123456789:;<=>?" +
	"@ABCDEFGHIJKLMNO" +
	"PQRSTUVWXYZ[\\]^_" +
	"`abcdefghijklmno" +
	"pqrstuvwxyz{|}~⌂" +

	"ÇüéâäàåçêëèïîìÄÅ" +
	"ÉæÆôöòûùÿÖÜ¢£¥€ƒ" +
	"áíóúñÑªº½⅓¼⅕⅙⅛«»" +
	"░▒▓│┤╡╢╖╕╣║╗╝╜╛┐" +
	"└┴┬├─┼╞╟╚╔╩╦╠═╬╧" +
	"╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀" +
	"αßΓπΣσµτΦΘΩδ∞φε∩" +
	"⁰¹²³⁴⁵⁶⁷⁸⁹ⁱⁿ⁽⁼⁾¤"
const dropChar = '⛔'

var chars []rune
	


type Dumper struct {
	w io.Writer
	offset int
	rhs []rune
}

func NewDumper(w io.Writer) *Dumper {
	return &Dumper{w, 0, make([]rune, 0, 16)}
}

func (d *Dumper) writeRhs() {
	fmt.Fprint(d.w, " ┆")
	fmt.Fprint(d.w, string(d.rhs))
	fmt.Fprintln(d.w, "┆")
}

func (d *Dumper) Close() {
	nout := (16 - (d.offset % 16)) % 16
	for i := 0; i < nout; i += 1 {
		fmt.Print("   ")
	}
	if nout > 7 {
		fmt.Print(" ")
	}
	if len(d.rhs) > 0 {
		d.writeRhs()
	}
	fmt.Fprintf(d.w, "%08x\n", d.offset)
}

func (d *Dumper) writeOut(s string, r rune, legit bool) {
	if d.offset % 16 == 0 {
		fmt.Fprintf(d.w, "%08x  ", d.offset)
	}
	
	fmt.Fprint(d.w, s)
	fmt.Fprint(d.w, " ")
	if legit {
		d.rhs = append(d.rhs, r)
		d.offset += 1
	}
	
	switch len(d.rhs) {
	case 8:
		fmt.Fprint(d.w, " ")
	case 16:
		d.writeRhs()
		d.rhs = d.rhs[:0]
	}
}

func (d *Dumper) DumpByte(b byte) {
	d.writeOut(fmt.Sprintf("%02x", b), chars[b], true)
}

func (d *Dumper) DumpDrop() {
	d.writeOut("--", dropChar, true)
}

func (d *Dumper) DumpString(s string) {
	for i := 0; i < len(s); i += 1 {
		d.DumpByte(s[i])
	}
}

func (d *Dumper) DumpGap(gaplen int) {
	for i := 0; i < gaplen; i += 1 {
		d.DumpDrop()
	}
}

func (d *Dumper) DumpGapstr(g *Gapstr) {
	lastpos := 0
	proc := func(pos int, s string) string {
		gap := pos - lastpos
		d.DumpGap(gap)
		d.DumpString(s)
		lastpos = gap + len(s)
		return s
	}
	_ = g.Map(proc)
}


func FdumpString(w io.Writer, s string) {
	d := NewDumper(w)
	d.DumpString(s)
	d.Close()
}

func FdumpGapstr(w io.Writer, g *Gapstr) {
	d := NewDumper(w)
	d.DumpGapstr(g)
	d.Close()
}

func DumpString(s string) {
	FdumpString(os.Stdout, s)
}

func DumpGapstr(g *Gapstr) {
	FdumpGapstr(os.Stdout, g)
}

func init() {
	chars = []rune(charset)
}

