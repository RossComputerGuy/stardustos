#!/usr/bin/env bash

if [ -n "$TRAVIS_BUILD_DIR" ]; then
	p="$TRAVIS_BUILD_DIR"
else
	p=$(dirname "$0")
	p=$(dirname "$p")
fi
p=$(readlink -f "$p")
docker run -v "$p":/usr/src -t stardustos /usr/src/scripts/build.sh "$@"
