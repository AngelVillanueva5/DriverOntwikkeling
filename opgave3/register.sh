#!/bin/bash

sudo insmod opgave_3_3.ko param=123
sudo mknod /dev/hello_driver c 500 1 -m 0666
