#!/usr/bin/env bash

set -xe

mkdir -p target

gcc -Wall -Wextra -Wconversion -pedantic -g src/mini-a-b.c -o target/mini-a-b
