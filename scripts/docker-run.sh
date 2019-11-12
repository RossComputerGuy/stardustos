#!/usr/bin/env bash

if [ -n "$TRAVIS_BUILD_DIR" ]; then
 p="$TRAVIS_BUILD_DIR"
else
  p="$(readlink -f $(dirname $(dirname $0)))"
fi

docker run -ti -v "$p":/usr/src -t stardustos /usr/src/scripts/build.sh
