all: run2

compr: daldon
	./daldon &&\
		fp8 -8 rules.c &&\
		fp8 -8 rules.h &&\
		ls -lS 2k rules.c.fp8 rules.h.fp8

run2: daldon
	./daldon 2>&1|tee tmp &&\
		diff -sq tmp tmp.ref

run: daldon
	time -p ./daldon &&\
		gcc -Wall -Wno-trigraphs -o rul -O2 rules.c &&\
		./rul >tmp && echo _______ &&\
		diff -s -q 10k tmp

mem: daldondbg
	valgrind --leak-check=full ./daldondbg

profile: daldondbg
	valgrind --tool=callgrind --dump-instr=yes ./daldondbg

dbg: daldondbg
	gdb -q -ex "run" ./daldondbg

daldondbg: daldon.c
	gcc -g -o daldondbg daldon.c nash/hash.c -lm &&\
		frama-c daldon.c

daldon: daldon.c
	gcc -Wall -o daldon -O2 daldon.c -lm nash/hash.c &&\
		uno daldon.c &&\
		frama-c daldon.c &&\
		clang daldon.c -lm nash/hash.c
