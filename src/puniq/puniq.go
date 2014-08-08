package main

import (
	"fmt"
	"os"
	"pcap"
)

func main() {
	r, err := pcap.NewReader(os.Stdin)
	if err != nil {
		panic(err)
	}

	w, err := pcap.NewWriter(os.Stdout)
	if err != nil {
		panic(err)
	}

	fmt.Println(r.Header)

	var lastframe *pcap.Frame = nil
	for {
		frame, err := r.Read()
		if err != nil {
			panic(err)
		}
		if frame == nil {
			break
		}

		if (lastframe != frame) {
			err = w.WriteFrame(*frame)
			if err != nil {
				panic(err)
			}

			lastframe = frame
		}
	}
}
