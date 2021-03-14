gcc -c shstacks.c -o shstacks.o 
ar rcs libshstacks.a shstacks.o 
gcc stack.c -o s -L. -lshstacks
gcc stack.c -o s2 -L. -lshstacks
