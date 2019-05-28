#!/bin/sh

cc -o test test.c

./test 0 &
./test 0 &
./test 0 &
./test 1 &
./test -1 &
