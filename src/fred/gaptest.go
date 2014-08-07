package main

import (
	"fmt"
	"netarch"
)

func main() {
	g := new(netarch.Gapstr)
	g = g.AppendString("hello")
	g = g.AppendGap(12)
	g = g.AppendString("world")
	fmt.Println("hi", g.String())
}
