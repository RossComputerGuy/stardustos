#!/usr/bin/env bash

p="$(dirname $0)"
p="$(dirname $p)"
docker build -t stardustos "$(readlink -f $p)"
