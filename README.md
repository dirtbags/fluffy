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


### skip: discard initial octets

Throws away some initial octets from stdin,
and sends the rest to stdout.
You could use `dd` for the same purpose.

This skip command:

	skip 5

Is equivalent to this `dd` command:

    dd skip=5 bs=1 status=none


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

The opposite of `unhex`.

The following is the equivalent of `cat`:

    hex | unhex


### printfesc: printf escape input

Reads octets,
writes a string suitable for copy-paste into printf.


### pyesc: python escape input

Escapes input octets for pasting into a python "print" statement.



