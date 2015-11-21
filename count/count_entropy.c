#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#define NDEBUG  // remove for debugging (turns on Array bound checks)
#include <assert.h>
#include <stdint.h>

#ifdef UNIX
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#endif

#define HASH_SIZE 256
#define BUF_SIZE 1024
main(int argc, char **argv) {
long unsigned int hash[HASH_SIZE];
int i, len;
unsigned char c;
	// Init hash
	for (i = 0; i < HASH_SIZE; i++)
		hash[i] = 0;
	// Fill it
    unsigned char *buf = malloc(BUF_SIZE);
    FILE *fp = fopen(argv[1], "r");
    long tot = 0;
    unsigned char uni = 0;
    double entropy = 0;
	while (len = fread(buf, sizeof(char), BUF_SIZE, fp)) {
        tot += len;
        while (len-- > 0)
            hash[buf[len]]++;
    }
	// report
    char most = 0;
    unsigned max = 0;
	for (i = 0; i < HASH_SIZE; i++) {
        double empiric_probability = (double)hash[i] / (double)tot;
        if (empiric_probability) {
            if (max < hash[i]) {
                most = i;
                max = hash[i];
            }
            ++uni;
            entropy -= (empiric_probability * log(empiric_probability) / log(2));
        }
    }
    printf("\nTotal symbols:\t%ld\nUnique symbols:\t%u\n"
            "Top count (%c):\t%u\nSymbol"
            " entropy:\t%f\nTotal entropy:\t%u\nHuffencoded:\t%d\nArith."
            " coded:\t%d\n",
            tot, uni, most, max, entropy, (int)(entropy * tot + 0.99999),
            (int)(tot + entropy * tot + 0.99999),
            (int)(tot / 10 + entropy * tot + 0.99999));
	return 0;
}
