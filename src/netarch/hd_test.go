package netarch

import (
)

func ExampleDumpString() {
	DumpString("hello world, I can write!")
	// Output: 
	// 00000000  68 65 6c 6c 6f 20 77 6f  72 6c 64 2c 20 49 20 63  ┆hello world, I c┆
	// 00000010  61 6e 20 77 72 69 74 65  21                       ┆an write!┆
	// 00000019
}

func ExampleDumpGapstr() {
	g := new(Gapstr)
	g = g.AppendString("Hell")
	g = g.AppendGap(3)
	g = g.AppendString("orld!")
	DumpGapstr(g)
	// Output:
	// 00000000  48 65 6c 6c -- -- -- 6f  72 6c 64 21              ┆Hell⛔⛔⛔orld!┆
	// 0000000c
}