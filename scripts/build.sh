#!/usr/bin/env bash

set -e

if ! [ -x "$(command -v mdl)" ]; then
	export PATH=$PATH:/home/ross/.gem/ruby/2.6.0/bin
fi

mkdir -p build
cd build
p=$(dirname "$0")
p=$(dirname "$p")
cmake -S "$p" "$@"
c=$(($(grep -P '^core id\t' /proc/cpuinfo | sort -u | wc -l) / 2))
if [ -z "$c" ] || [ "$c" -lt 1 ]; then
	c=2
fi
make -j "$c"
