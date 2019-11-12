#!/usr/bin/env bash

set -e

mkdir -p build
cd build
p="$(dirname $0)"
cmake -S "$(dirname $p)"
make
