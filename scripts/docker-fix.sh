#!/usr/bin/env bash

p=$(dirname "$0")
p=$(dirname "$p")
p=$(readlink -f "$p")

id=$(id -u)
line=$(grep "RUN useradd" "$p/Dockerfile")
rep=$(echo "$line" | sed -re "s/(-u)[^=]*$/\1 $id/")
sed "s|$line|$rep|g" "$p/Dockerfile"
