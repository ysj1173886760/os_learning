### ASCII-OS

A toy operating system which can only play games

support the basic interface of drawing texts

ascii-os is aimed to run on raspi3, other platform is not supported, maybe raspi4 can worked well

it doesn't own the must-have component of a normal os e.g. file system, virtual memory

###### build

after clone the repo to local

you have to configure it first, open Makefile, find GCCPATH, change it to your local path of cross compiler

then type `make`

then you need to have qemu which can support simulate raspi3

my version is 3.0.0

then type `qemu-system-aarch64 -kernel kernel8.img -M raspi3 -serial stdio`

then you can run play your game

##### note

note that we have redirected the UART to our stdio when we launch the qemu, so you have to use your terminal/console to control the game

currently only support Tetris and Bird

Welcome to contribute