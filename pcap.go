package main
//package pcap

import (
	"time"
	"os"
	"fmt"
	"io"
	"encoding/binary"
)

const MAGIC = 0xa1b2c3d4
const MAXFRAME = 9000

const LINKTYPE_ETHERNET = 1
const LINKTYPE_RAW = 101

type Frame struct {
	ts time.Time
	caplen uint32
	framelen uint32
	payload string
}

type FileHeader struct {
	VersionMajor uint16
	VersionMinor uint16
	ThisZone int32
	SigFigs uint32
	SnapLen int32
	LinkType int32
}

type PcapFile struct {
	order binary.ByteOrder
	header FileHeader
}

type Reader struct {
	PcapFile
	r io.Reader
}

type Writer struct {
	PcapFile
	w io.Writer
}

func NewReader(r io.Reader) (*Reader, error) {
	var h FileHeader
	var magic uint32
	var order binary.ByteOrder = binary.BigEndian

	if err := binary.Read(r, order, &magic); err != nil {
		return nil, err
	}
	if magic != MAGIC {
		order = binary.LittleEndian
	}

	if err := binary.Read(r, order, &h); err != nil {
		return nil, err
	}

	ret := &Reader{PcapFile{order, h}, r}

	return ret, nil
}

func main() {
	r, err := NewReader(os.Stdin)
	if err != nil {
		panic(err)
	}

	fmt.Println(r.order, r.header)
}
