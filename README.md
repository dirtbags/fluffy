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


How To Build
------------

This assumes you're on an Ubuntu system,
or otherwise have `$HOME/bin` in your `PATH` environment variable.

    git clone https://github.com/dirtbags/fluffy
	cd fluffy
	make DESTDIR=$HOME/bin install


Programs
--------

### hd: Hex Dump

Like the normal hd,
but with unicode characters to represent all 256 octets,
instead of using "." for unprintable characters.


### unhex: unescape hex

Reads ASCII hex codes on stdin,
writes those octets to stdout.

The following pipe is equivalent to "cat":

    ./hd | cut -b 11-58 | ./unhex


### xor: xor mask octets

Applies the given mask as an xor to input.
The mask will be repeated,
so for a 1-value mask, every octet is xored against that value.
For a 16-value mask, the mask is applied to 16-octet chunks at a time.

The "-x" option treats values as hex.

The following pipe is equivalent to "cat":

	./xor 42 | ./xor -x 2A


### pcat: print text representation of pcap file

Prints a (lossy) text representation of a pcap file to stdout.
This program is the keystone of the Fluffy Suite.
By representing everything as text,
programmers can use any number of standard Unix text processing tools,
such as sed, awk, cut, grep, or head.


### pmerge: merge pcap files 

Takes a list of pcap files, assuming they are sorted by time
(you would have to work hard to create any other kind),
and merges them into a single sorted output.


### printfesc: printf escape input

Reads octets,
writes a string suitable for copy-paste into printf.


### puniq: omit repeated frames

Removes duplicate frames from input, 
writing to output.


### pyesc: python escape input

Escapes input octets for pasting into a python "print" statement.



