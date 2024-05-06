#!/bin/bash

sudo insmod leddriver.ko param=123
sudo mknod /dev/hello_driver c 500 1 -m 0666
