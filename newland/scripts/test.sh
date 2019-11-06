#!/usr/bin/env bash

EMULATOR=$1
PROJECT_BINARY_DIR=$2

shift 2

timeout 30s ${EMULATOR} -kernel ${PROJECT_BINARY_DIR}/kernel.elf -nographic -display none -serial stdio $@
RETVAL=$?
if [ $RETVAL == 124 ]; then
  exit 0
else
  exit $RETVAL
fi
