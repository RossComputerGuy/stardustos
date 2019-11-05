#!/usr/bin/env bash

timeout 30s $1 -kernel $2/kernel.elf -nographic -display none -serial stdio
RETVAL=$?
if [ $RETVAL == 124 ]; then
  exit 0
else
  exit $RETVAL
fi