#! /bin/sh

tests=0
successes=0
failures=0

H () {
    section="$*"
    printf "\n%-20s " "$*"
}

title() {
    thistest="$1"
    tests=$(expr $tests + 1)
}

successes=0
pass () {
    printf '.'
    successes=$(expr $successes + 1)
}

failures=0
fail () {
    printf '(%s)' "$thistest"
    failures=$(expr $failures + 1)
}


H "xor"

title "single-byte decimal"
printf 'hello' | ./xor 22 | grep -q '~szzy' && pass || fail

title "single-byte hex"
printf 'hello' | ./xor 0x16 | grep -q '~szzy' && pass || fail

title "single-byte -x"
printf 'hello' | ./xor -x 16 | grep -q '~szzy' && pass || fail


H "hex/unhex"

title "hex"
printf ' hello' | ./hex | grep -q '68 65 6c 6c 6f' && pass || fail

title "unhex"
echo '68 65 6c 6c 6f' | ./unhex | grep -q 'hello' && pass || fail


H "slice"

title "slice begin"
printf 'hello' | ./slice 2 | grep -qx 'llo' && pass || fail

title "slice begin + end"
printf 'hello' | ./slice 2 4 | grep -qx 'll' && pass || fail

title "slice begin + end + begin"
printf 'hello' | ./slice 0 2 4 | grep -qx 'heo' && pass || fail


H "hd"

title "hd short"
printf 'hello' | ./hd | grep -qx '00000000  68 65 6c 6c 6f.*hello' && pass || fail


H "Everything else"

title "entropy"
printf 'hello' | ./entropy | grep -q 1.921928 && pass || fail

title "pyesc"
printf '\0hello\n' | ./pyesc | grep -xq '.x00hello.n' && pass || fail

title "freq"
printf 'hello' | ./freq | grep -xq '2 6c l' && pass || fail

title "histogram"
printf 'hello' | ./freq | ./histogram | grep -xq '6c l ## 2' && pass || fail

echo
echo
echo "$successes of $tests tests passed ($failures failed)."

exit $failures