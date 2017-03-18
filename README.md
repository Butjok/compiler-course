Compile runtime math library:

    gcc -c lm.s
    
Compile compiler:

    g++ *.cpp -o mycc -O4

    ./mycc c.txt > .c.txt.s
    gcc -c .c.txt.s -o c.o

Usage:

    ./cc file.txt && ./a.out
    
