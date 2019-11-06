#!/usr/bin/env bash

set -e

EMULATOR=$1
PROJECT_BINARY_DIR=$2

shift 2

timeout 30s $(dirname $0)/run.sh ${EMULATOR} ${PROJECT_BINARY_DIR} -nographic -display none $@
RETVAL=$?
if [ $RETVAL == 124 ]; then
  exit 0
else
  exit $RETVAL
fi
