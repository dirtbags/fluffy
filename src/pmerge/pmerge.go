package main

import (
	"fmt"
	"flag"
	//"os"
	//"pcap"
)

func main() {
	flag.Parse()
	
	for i := 0; i < flag.NArg(); i += 1 {
		fn := flag.Arg(i)
		fmt.Println(fn)
	}
}
