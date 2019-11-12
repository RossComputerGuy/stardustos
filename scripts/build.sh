#!/usr/bin/env bash

set -e

mkdir -p build
cd build
cmake -S "$(dirname $(dirname $0))"
make
