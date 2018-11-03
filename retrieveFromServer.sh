#!/usr/bin/env bash

echo Enter the path of the source file: 

read $source

echo Enter the path of the target directory:

read $target

scp -i ubuntu.pem ubuntu@ec2-13-229-125-163.ap-southeast-1.compute.amazonaws.com:~/$source  $target
