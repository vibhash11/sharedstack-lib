#!/bin/sh
gcc -c -Wall -Werror -fpic shstacks.c
gcc -shared -o libshstacks.so shstacks.o
rm shstacks.o
sudo cp ./libshstacks.so /usr/lib
sudo chmod 0755 /usr/lib/libshstacks.so
sudo ldconfig
sudo ldconfig -p | grep shstacks
unset LD_LIBRARY_PATH
gcc stack.c -o s -lshstacks
gcc stack.c -o s2 -lshstacks
