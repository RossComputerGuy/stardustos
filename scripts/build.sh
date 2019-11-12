#!/usr/bin/env bash

set -e

mkdir -p build
cd build
p="$(dirname $0)"
p="$(dirname $p)"
cmake -S "$p"
make
