The Fluffy Suite
============

Fluffy was begun in April 2011 in Tennessee,
as a replacement for the aging "dirtbags.ip" codebase.
It is comprised of multiple small standalone binaries,
which are meant to be chained together,
either on the command-line or from a shell script,
to create a more powerful (and specific) piece of software.

Usually, a program expects input on stdin,
and produces output on stdout.
Flags are sparse by design.

Fluffy source code is purposefully spartan and easy to audit.
Forks are encouraged,
please let me know if you make one.


How To Build
------------

	curl -L https://github.com/dirtbags/fluffy/archive/master.tar.gz | tar xzvf -
	cd fluffy-master
	make DESTDIR=$HOME install

On an Ubuntu system,
you may need to log out, and log back in,
before the utilities are in your path.

On a non-Ubuntu system,
you may need to edit your `.bashrc` to add `$HOME/bin` to your `PATH`
environment variable.


Programs
--------

### hd: Hex Dump

Like the normal hd,
but with unicode characters to represent all 256 octets,
instead of using "." for unprintable characters.

	$ printf "\0\x01\x02\x03\x30\x52\x9a" | hd
	00000000  00 01 02 03 30 52 9a                              ┆·☺☻♥0RÜ┆
	00000007


### unhex: unescape hex

Reads ASCII hex codes on stdin,
writes those octets to stdout.

	$ echo 68 65 6c 6c 6f 0a | unhex
	hello


### xor: xor mask octets

Applies the given mask as an xor to input.
The mask will be repeated,
so for a 1-value mask, every octet is xored against that value.
For a 16-value mask, the mask is applied to 16-octet chunks at a time.

The "-x" option treats values as hex.

	$ printf 'hello' | xor 22; echo
	~szzy
	$ printf 'hello' | xor 0x16; echo
	~szzy
	$ printf 'hello' | xor -x 16; echo
	~szzy
	$ printf 'bbbbbb' | xor 1 0; echo
	cbcbcb


### skip: discard initial octets

Throws away some initial octets from stdin,
and sends the rest to stdout.

You could use `dd` for the same purpose.

	$ echo abcdefgh | dd skip=5 bs=1 status=none
	fgh
	$ echo abcdefgh | skip 5
	fgh


### pcat: print text representation of pcap file

Prints a (lossy) text representation of a pcap file to stdout.

This program is the keystone of the Fluffy Suite.
By representing everything as text,
programmers can use any number of standard Unix text processing tools,
such as sed, awk, cut, grep, or head.

Output is tab-separated, of the format:

    timestamp protocol options src dst payload

Frequently you are only interested in the payload,
so you can run pcat like:

    cat myfile.pcap | pcat | cut -f 6

Remember the `unhex` program,
which will convert payloads to an octet stream,
after you have done any maniuplations you want.


### pmerge: merge pcap files 

Takes a list of pcap files, assuming they are sorted by time
(you would have to work hard to create any other kind),
and merges them into a single sorted output.


### puniq: omit repeated frames

Removes duplicate frames from input, 
writing to output.


### hex: hex-encode input

The opposite of `unhex`:
encoding all input into a single output line.

	$ printf "hello\nworld\n" | hex
	68 65 6c 6c 6f 0a 77 6f  72 6c 64 0a


### pyesc: python escape input

Escapes input octets for pasting into a python "print" statement.
Also suitable for use as a C string,
a Go string,
and many other languages' string literals.

	$ printf "hello\nworld\n" | pyesc
	hello\nworld\n
