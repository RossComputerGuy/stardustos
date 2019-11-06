#!/usr/bin/env bash

set -e

EMULATOR=$1
PROJECT_BINARY_DIR=$2

shift 2

$(dirname $0)/run.sh ${EMULATOR} ${PROJECT_BINARY_DIR} -nographic -display none $@ & sleep 10 ; kill $!
