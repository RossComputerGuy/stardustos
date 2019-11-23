#!/usr/bin/env bash

tmpld=$(mktemp)
set -e
function finish {
	rm -rf "${tmpld}" deflink.test.c a.out
}
trap finish EXIT
echo "int main(int argc, char** argv) { return 0; }" > deflink.test.c
if [ -z "$CC" ]; then
	export CC=gcc
fi
"$CC" "${CFLAGS[@]}" -Wl,-verbose deflink.test.c | sed -ne '/using internal linker script:/,$ p' | sed '1,2d' | sed -e's/[[\n\n]]*$//' >"${tmpld}"
n=$(grep -n '==================================================' "${tmpld}" | cut -f1 -d ':')
n="$((n - 3))"
head -n "$n" "${tmpld}"