#!/bin/bash

sudo insmod platformdriver.ko param=123
sudo mknod /dev/gpio-extern c 500 1 -m 0666
