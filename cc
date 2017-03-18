#!/bin/sh

cat stdlib.txt $1 > .$1 && ./mycc .$1 > .$1.s && gcc .$1.s lm.o c.o -O4
