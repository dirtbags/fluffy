package main
//package pcap

import (
	"time"
	"os"
	"fmt"
	"io"
	"encoding/binary"
)

const MAXFRAME = 9000
const MAGIC_BE = "\xa1\xb2\xc3\xd4"
const MAGIC_LE = "\xd4\xc3\xb2\xa1"

const LINKTYPE_ETHERNET = 1
const LINKTYPE_RAW = 101

type FileHeader struct {
	VersionMajor uint16
	VersionMinor uint16
	ThisZone int32
	SigFigs uint32
	SnapLen uint32
	LinkType uint32
}

type PcapFile struct {
	order binary.ByteOrder
	header FileHeader
}

type FrameHeader struct {
	Sec uint32
	Usec uint32
	Caplen uint32
	Framelen uint32
}

func (h *FrameHeader) Time() time.Time {
	return time.Unix(int64(h.Sec), int64(h.Usec) * 1000)
}

type Frame struct {
	FrameHeader
	payload []byte
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
	var order binary.ByteOrder
	magic := make([]byte, 4)

	if err := r.Read(&magic); err != nil {
		return nil, err
	}
	switch (magic) {
	case MAGIC_BE:
		order = binary.BigEndian
	case MAGIC_LE:
		order = binary.LittleEndian
	default:
		return nil, fmt.Errorf("Cannot determine endianness")
	}
	

	if err := binary.Read(r, order, &h); err != nil {
		return nil, err
	}

	ret := &Reader{PcapFile{order, h}, r}

	return ret, nil
}

func (r *Reader) Read() (*Frame, error) {
	var h FrameHeader

	if err := binary.Read(r.r, r.order, &h); err != nil {
		return nil, err
	}

	payload := make([]byte, h.Caplen)
	l, err := r.r.Read(payload)
	if err != nil {
		return nil, err
	}
	if uint32(l) < h.Caplen {
		return nil, fmt.Errorf("Short read: %d (wanted %d)", l, h.Caplen)
	}

	ret := &Frame{h, payload}
	return ret, nil
}

func main() {
	r, err := NewReader(os.Stdin)
	if err != nil {
		panic(err)
	}

	fmt.Println(r.order, r.header)

	for ;; {
		frame, err := r.Read()
		if err != nil {
			panic(err)
		}
		if frame == nil {
			break
		}
		fmt.Println("hi")
		fmt.Println(frame)
	}
}
