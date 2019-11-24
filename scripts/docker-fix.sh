#!/usr/bin/env bash

p=$(dirname "$0")
p=$(dirname "$p")
p=$(readlink -f "$p")
line=$(grep "RUN useradd" "$p/Dockerfile")
rep=$(echo "$line" | sed -r "s/(-u)[^=]*$/\1 $(id -u)/")
sed -i "s|$line|$rep|g" "$p/Dockerfile"
