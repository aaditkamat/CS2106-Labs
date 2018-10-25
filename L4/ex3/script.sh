#!/bin/bash

rm -rf a.out

make

./a.out < test1.in > test1.out
./a.out < test2.in > expected2.out

vim -o expected1.out expected2.out
