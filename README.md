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
============

## From Source

    curl -L https://github.com/dirtbags/fluffy/archive/master.tar.gz | tar xzvf -
    cd fluffy-master
    make DESTDIR=$HOME install
    PATH=$HOME/bin:$PATH

On an Ubuntu system,
you may need to log out, and log back in,
before the utilities work in all terminals.

On a non-Ubuntu system,
you may need to edit your `.bashrc` to add `$HOME/bin` to your `PATH`
environment variable.


Forks and Packages
==================

## Ubuntu

pi-rho, a network archaeology instructor,
has forked these tools,
added command-line options,
manual pages,
and packaged them for Ubuntu.

This fork is mostly compatible with these tools,
but there are a few subtle differences.
If you are installing these for Cyber Fire,
you should probably stick with a source install.

[pi-rho's packages](https://launchpad.net/~pi-rho/+archive/ubuntu/security)

## Arch Linux

The AUR package [`fluffy-git`](https://aur.archlinux.org/packages/fluffy-git/)
builds against the latest revision and installs it to `/usr/bin`.
This was packaged by Cyber Fire attendee AGausmann.
Thanks!


Programs
========

## hd: Hex Dump

Like the normal hd,
but with unicode characters to represent all 256 octets,
instead of using "." for unprintable characters.

    $ printf "\0\x01\x02\x03\x30\x52\x9a" | hd
    00000000  00 01 02 03 30 52 9a                              □☺☻♥0RÜ
    00000007

Also like the normal hd,
this one will print an asterisk if the preceding 16 octets are repeated.
Use the offset printed next to determine how many repeats you have.

    printf '%64s' hello | hd
    00000000  20 20 20 20 20 20 20 20  20 20 20 20 20 20 20 20                  
    *
    00000030  20 20 20 20 20 20 20 20  20 20 20 68 65 6c 6c 6f             hello
    00000040


## unhex: unescape hex

Reads ASCII hex codes on stdin,
writes those octets to stdout.

    $ echo 68 65 6c 6c 6f 0a | unhex
    hello


## xor: xor octets

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


## slice: slice octet stream

Slices up input octet stream,
similar to Python's slice operation.

    ~/src/fluffy $ printf '0123456789abcdef' | slice 2; echo
    23456789abcdef
    ~/src/fluffy $ printf '0123456789abcdef' | slice 2 6; echo
    2345
    ~/src/fluffy $ printf '0123456789abcdef' | slice 2 6 8; echo
    234589abcdef
    ~/src/fluffy $ printf '0123456789abcdef' | slice 2 6 8 0xa
    234589


## pcat: print text representation of pcap file

Prints a (lossy) text representation of a pcap file to stdout.

This program is the keystone of the Fluffy Suite.
By representing everything as text,
programmers can use any number of standard Unix text processing tools,
such as sed, awk, cut, grep, or head.

Output is tab-separated, of the format:

    timestamp protocol src dst options payload

Frequently you are only interested in the payload,
so you can run pcat like:

    cat myfile.pcap | pcat | cut -f 6

Remember the `unhex` program,
which will convert payloads to an octet stream,
after you have done any maniuplations you want.


## pmerge: merge pcap files

Takes a list of pcap files, assuming they are sorted by time
(you would have to work hard to create any other kind),
and merges them into a single sorted output.


## puniq: omit repeated frames

Removes duplicate frames from input,
writing to output.


## hex: hex-encode input

The opposite of `unhex`:
encoding all input into a single output line.

This differs from `hexdump` in the following ways:

* All input is encoded into a single line of output
* Does not output offsets
* Does not output glyph representations of octets

In other words: you can feed `hex` output into `unhex` with no manipulations.

    $ printf "hello\nworld\n" | hex
    68 65 6c 6c 6f 0a 77 6f  72 6c 64 0a
    $ printf A | hex
    41


## entropy: compute shannon entropy

Displays the Shannon entropy of the input.

    ~/src/fluffy $ echo -n a | ./entropy
    0.000000
    ~/src/fluffy $ echo -n aaaaaaaaa | ./entropy
    0.000000
    ~/src/fluffy $ echo -n aaaaaaaaab | ./entropy
    0.468996
    ~/src/fluffy $ echo -n aaaaaaaaabc | ./entropy
    0.865857


## pyesc: python escape input

Escapes input octets for pasting into a python "print" statement.
Also suitable for use as a C string,
a Go string,
and many other languages' string literals.

    $ printf "hello\nworld\n" | pyesc
    hello\nworld\n


## octets: display all octets

Shows all octets from `00` to `ff` in a hex dump.
This is occasionally more helpful than `man ascii`.

    $ octets
    00000000  00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  □☺☻♥♦♣♠•◘○◙♂♀♪♫☼
    00000010  10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  ►◄↕‼¶§▬↨↑↓→←∟↔▲▼
    00000020  20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f   !"#$%&'()*+,-./
    00000030  30 31 32 33 34 35 36 37  38 39 3a 3b 3c 3d 3e 3f  0123456789:;<=>?
    00000040  40 41 42 43 44 45 46 47  48 49 4a 4b 4c 4d 4e 4f  @ABCDEFGHIJKLMNO
    00000050  50 51 52 53 54 55 56 57  58 59 5a 5b 5c 5d 5e 5f  PQRSTUVWXYZ[\]^_
    00000060  60 61 62 63 64 65 66 67  68 69 6a 6b 6c 6d 6e 6f  `abcdefghijklmno
    00000070  70 71 72 73 74 75 76 77  78 79 7a 7b 7c 7d 7e 7f  pqrstuvwxyz{|}~⌂
    00000080  80 81 82 83 84 85 86 87  88 89 8a 8b 8c 8d 8e 8f  ÇüéâäàåçêëèïîìÄÅ
    00000090  90 91 92 93 94 95 96 97  98 99 9a 9b 9c 9d 9e 9f  ÉæÆôöòûùÿÖÜ¢£¥₧ƒ
    000000a0  a0 a1 a2 a3 a4 a5 a6 a7  a8 a9 aa ab ac ad ae af  áíóúñÑªº¿⌐¬½¼¡«»
    000000b0  b0 b1 b2 b3 b4 b5 b6 b7  b8 b9 ba bb bc bd be bf  ░▒▓│┤╡╢╖╕╣║╗╝╜╛┐
    000000c0  c0 c1 c2 c3 c4 c5 c6 c7  c8 c9 ca cb cc cd ce cf  └┴┬├─┼╞╟╚╔╩╦╠═╬╧
    000000d0  d0 d1 d2 d3 d4 d5 d6 d7  d8 d9 da db dc dd de df  ╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀
    000000e0  e0 e1 e2 e3 e4 e5 e6 e7  e8 e9 ea eb ec ed ee ef  αßΓπΣσµτΦΘΩδ∞φε∩
    000000f0  f0 f1 f2 f3 f4 f5 f6 f7  f8 f9 fa fb fc fd fe ff  ≡±≥≤⌠⌡÷≈°∙·√ⁿ²■¤
    00000100


Example Recipes
===============


## Brute force single-byte xor

    for i in $(seq 255); do cat data | xor $i; done


## Pretty xor brute force

For each attempt, display the value used in the xor, and hexdump the result

    for i in $(seq 255); do printf "=== %02x\n" $i; cat data | xor $i | hd; done


## Brute force xor of base64-encoded data

Same pretty-print as before, and also pipe to `less` so we can page through it.

    for i in $(seq 255); do
      printf "=== %02x\n" $i; cat data.txt | base64 -d | xor $i | hd
    done | less

 
## Protocol manipulation

For each ICMP packet, drop the first 5 octets, and base64-decode the remainder, preserving conversation chunks

    cat input.pcap | pcat | grep ICMP | while read ts proto src dst payload; do
      printf "%s -> %s (%s)\n" $src $dst $ts
      echo $payload | unhex | slice 5 | base64 -d | hd
    done


## Elementary protocol analysis framework

This merges (by time) `file1.pcap` and `file2.pcap`,
decoding payloads from each one,
hex dumping payloads,
and displaying meta information about each.
It displays information conversationally,
sort of like wireshark's "Follow TCP Stream",
but with more details about meta-information.

    ./pmerge file1.pcap file2.pcap | ./pcat | while read ts proto src dst payload; do
        when=$(TZ=Z date -d @${ts%.*} "+%Y-%m-%d %H:%M:%S")
        printf "Packet %s None: None\n" $proto
        printf "    %s -> %s (%s)\n" ${src%,*} ${dst%,*} "$when"
        echo $payload | ./unhex | ./hd
        echo
    done
