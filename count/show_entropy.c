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

double entropy(long unsigned tot, long unsigned *hash) {
    if (!tot) return 0;
    int i;
    double res = 0;
	for (i = 0; i < HASH_SIZE; i++) {
        int x = hash[i];
        if (x) res -= (x * log((double)x / (double)tot) / log(2));
    }
    return res;
}

main(int argc, char **argv) {
    long unsigned int *hash1 = calloc(HASH_SIZE, sizeof(long unsigned));
    long unsigned int *hash2 = calloc(HASH_SIZE, sizeof(long unsigned));
    int len;
    unsigned char c;
    unsigned char *buf = malloc(BUF_SIZE);
    FILE *fp = fopen(argv[1], "r");
    // Fill hash
    long unsigned tot1 = 0;
    while (len = fread(buf, sizeof(char), BUF_SIZE, fp)) {
        tot1 += len;
        while (len-- > 0)
            hash1[buf[len]]++;
    }
   double orig = entropy(tot1, hash1);
    rewind(fp);
    long unsigned pos = 0;
    long unsigned tot2 = 0;
    while (len = fread(buf, sizeof(char), BUF_SIZE, fp)) {
        int i = -1;
        while (++i < len) {
            --tot1;
            --hash1[buf[i]];
            ++tot2;
            ++hash2[buf[i]];
            double x = entropy(tot1, hash1);
            printf("%lu\t%f\n", pos++, orig / (entropy(tot1, hash1) + entropy(tot2, hash2)) - 1.0);
        }
    }
    return 0;
}
