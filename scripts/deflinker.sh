#!/usr/bin/env bash

tmpld=$(mktemp)
set -e
function finish {
	rm -rf "${tmpld}" deflink.test.c a.out
}
trap finish EXIT
cat << EOF > deflink.test.c
int main(int argc, char** argv) { return 0; }
EOF
gcc -Wl,-verbose deflink.test.c | sed -ne '/using internal linker script:/,$ p' | sed '1,2d' | sed -e's/[[\n\n]]*$//' >"${tmpld}"
n=$(grep -n '==================================================' "${tmpld}" | cut -f1 -d ':')
n="$(($n - 3))"
head -n "$n" "${tmpld}"