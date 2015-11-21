all: count_chars count_entropy show_entropy cr
	./count_chars ../enwik8 >char.count
	./count_entropy ../enwik8 
	#./show_entropy abc >e.abc && echo "plot 'e.abc'"|gnuplot -p
	./cr abc.bwt >e.g && echo "plot 'e.g' with lines"|gnuplot -p
count_chars: count_chars.c
	gcc -O2 -o count_chars count_chars.c ../hash/hash.c

count_entropy: count_entropy.c
	gcc -O2 -o count_entropy count_entropy.c -lm ../hash/hash.c

show_entropy: show_entropy.c
	gcc -O2 -o show_entropy show_entropy.c -lm ../hash/hash.c

cr: cr.c
	gcc -O2 -o cr cr.c -lm ../hash/hash.c
