#!/usr/bin/env bash

docker build -t stardustos "$(readlink -f $(dirname $(dirname $0)))"
