#!/usr/bin/env bash

if [ -n "$TRAVIS_BUILD_DIR" ]; then
 p="$TRAVIS_BUILD_DIR"
else
  p="$(dirname $0)"
  p="$(dirname $p)"
fi

docker run -ti -v "$(readlink -f $p)":/usr/src -t stardustos /usr/src/scripts/build.sh
