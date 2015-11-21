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
	while (len = fread(buf, sizeof(char), BUF_SIZE, fp))
        while (len-- > 0)
            hash[buf[len]]++;
	// report
	for (i = 0; i < HASH_SIZE; i++)
		printf("%d %ld\n", i, hash[i]);
	return 0;
}
