#!/usr/bin/env bash

set -e

EMULATOR=$1
PROJECT_BINARY_DIR=$2

shift 2

${EMULATOR} -kernel ${PROJECT_BINARY_DIR}/kernel.elf $@
