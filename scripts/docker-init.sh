#!/usr/bin/env bash

p=$(dirname "$0")
p=$(dirname "$p")
p=$(readlink -f "$p")
docker build -t stardustos "$p" "$@"
