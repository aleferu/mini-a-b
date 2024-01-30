#!/usr/bin/env bash

set -xe

mkdir -p target

gcc -Wall -Wextra -Wconversion -pedantic -g src/*.c -Isrc/ -o target/mini-a-b
