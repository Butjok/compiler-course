gcc -c lm.s

g++ *.cpp -o mycc -O4

./mycc c.txt > .c.txt.s
gcc -c .c.txt.s -o c.o

Usage:
./cc file.txt
./a.out