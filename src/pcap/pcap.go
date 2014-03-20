package pcap

import (
	"time"
	"fmt"
	"io"
	"encoding/binary"
)

const MAXFRAME = 9000
const MAGIC_BE = "\xa1\xb2\xc3\xd4"
const MAGIC_LE = "\xd4\xc3\xb2\xa1"
const VERSION_MAJ = 2
const VERSION_MIN = 4

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
	Header FileHeader
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
	Header FrameHeader
	Payload string
}

func (f *Frame) Time() time.Time {
	return f.Header.Time()
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

	n, err := r.Read(magic)
	if err != nil {
		return nil, err
	}
	if (n < 4) {
		return nil, fmt.Errorf("Cannot read pcap header")
	}
	
	switch (string(magic)) {
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
	
	if (h.VersionMajor != VERSION_MAJ) || (h.VersionMinor != VERSION_MIN) {
		return nil, fmt.Errorf("Cannot handle pcap file version %d.%d", h.VersionMajor, h.VersionMinor)
	}

	ret := &Reader{PcapFile{order, h}, r}

	return ret, nil
}

func (r *Reader) Read() (*Frame, error) {
	var h FrameHeader

	err := binary.Read(r.r, r.order, &h)
	if err == EOF {
		return nil, nil
	} 
	if err != nil {
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

	ret := &Frame{h, string(payload)}
	return ret, nil
}

func NewWriterLinkType(w io.Writer, linkType uint32) (*Writer, error) {
	order := binary.BigEndian
	h := &FileHeader{VERSION_MAJ, VERSION_MIN, 0, 0, 9000, linkType}
	
	n, err := w.Write([]byte(MAGIC_BE))
	if err != nil {
		return nil, err
	}
	if n != len(MAGIC_BE) {
		return nil, fmt.Errorf("Short write outputting header")
	}
	if err := binary.Write(w, order, h); err != nil {
		return nil, err
	}
	
	ret := &Writer{PcapFile{order, *h}, w}
	
	return ret, nil
}

func NewWriter(w io.Writer) (*Writer, error) {
	return NewWriterLinkType(w, 1)
}

func (w *Writer) WriteFrame(frame Frame) (error) {
	if err := binary.Write(w.w, w.order, frame.Header); err != nil {
		return err
	}
	n, err := w.w.Write([]byte(frame.Payload))
	if err != nil {
		return err
	}
	if n < len(frame.Payload) {
		return fmt.Errorf("Short write")
	}
	
	return nil
}

