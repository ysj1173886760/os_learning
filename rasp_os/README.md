### ASCII-OS

A toy operating system which can only play games

support the basic interface of drawing texts

ascii-os is aimed to run on raspi3, other platform is not supported, maybe raspi4 can worked well

it doesn't own the must-have component of a normal os e.g. file system, virtual memory

###### build

after clone the repo to local

you have to configure it first, open Makefile, find GCCPATH, change it to your local path of cross compiler

then type `make`

then you need to have qemu which can support simulating raspi3

my version here is 3.0.0

then type `qemu-system-aarch64 -kernel kernel8.img -M raspi3 -serial stdio`

then you can run play your game

##### note

note that we have redirected the UART to our stdio when we launch the qemu, so you have to use your terminal/console to control the game

currently only support Tetris and Bird

Welcome to contribute

#### KEYMAP

actually you can look into the source code yourself

but here we provided some necessary stuff here

Tetris: 
space -> rotate
a -> left
d -> right
s -> down

Flappy Bird:
space -> jump

BF:
`#` represent insert mode
`@` represent console mode

in insert mode, you can use backspace, return, arrow key as normal.
press to switch back to console mode

in console mode, `hjkl` can move the cursor
q -> quit
r -> run program
i -> switch to insert mode

##### Update

###### 0.0.1

bring BF interpreter and editor to ASCII-os

currently, i just use vim-like style to control the editor

i didn't test other programs but the routine [here](http://fatiherikli.github.io/brainfuck-visualizer/)

at least it can print hello world, i think there maybe some problems about the input, further test is needed

##### TODO

clean code, there are so many magic number here
mvprintw api is kind of strange, we need to change
port some easy graphic library to ASCII-os