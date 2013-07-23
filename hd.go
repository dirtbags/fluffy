package main

import (
	"bytes"
	"fmt"
	"os"
	"io"
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

func main() {
	offset := 0
	b := make([]byte, 16)
	lb := make([]byte, 16)
	chars := []rune(charset)
	skipping := false

	for ;; {
		n, err := os.Stdin.Read(b)
		if err != nil {
			if err != io.EOF {
				fmt.Println("read failed:", err)
			}
			break
		}
		
		if (offset > 0) && (bytes.Equal(lb, b)) {
			if ! skipping {
				fmt.Println("*")
			}
			skipping = true
			continue
		}
		copy(lb, b)
		skipping = false
		
		fmt.Printf("%08x ", offset)
		offset += n
		
		for i := 0; i < 16; i += 1 {
			if i % 8 == 0 {
				fmt.Printf("  ")
			}
			if i < n {
				fmt.Printf("%02x ", b[i])
			} else {
				fmt.Printf("   ")
			}
		}
		
		fmt.Printf("ǀ")
		for i := 0; i < n; i += 1 {
			fmt.Printf("%c", chars[b[i]])
		}
		fmt.Printf("ǀ\n")
	}
	
	fmt.Printf("%08x\n", offset)
}
