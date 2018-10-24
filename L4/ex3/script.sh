#!/bin/bash

rm -rf a.out

make

./a.out < test2.in > expected2.out

vim expected2.out
