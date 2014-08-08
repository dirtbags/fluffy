package main

import (
	"fmt"
	"netarch"
)

func main() {
	g := new(netarch.Gapstr)
	g = g.AppendString("hello")
	g = g.AppendGap(10)
	g = g.AppendString(" world")
	
	fmt.Println("hi", g.String())
	fmt.Printf("%v\n", g)
	fmt.Printf("%#v\n", g.Slice(0, 4))
	fmt.Printf("%#v\n", g.Slice(4, 8))
	fmt.Printf("%#v\n", g.Slice(8, 12))
	fmt.Printf("%#v\n", g.Slice(12, 16))
}
