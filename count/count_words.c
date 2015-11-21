#include "preprocess.h"
#include "hash/sprite.h"
#include "hash/hash.h"


// Hash Tables
char key_buffer[4];
char word_buffer[LONGEST_WORD];
static Hash_Table short2long;
static Hash_Table long2short;

int char2int(char c) {
	int ic = (int) c;
	return ic - 65 - ((ic > 96)? 6: 0);
}

void hash_put(int key_len, int val_len) {
	Hash_Entry *entry;
	Boolean flag;
	// Add \N to make string-compatible values
	word_buffer[val_len++] = 0;
	key_buffer[key_len++] = 0;
	// Allocate and fill the key.
	size_t klen = key_len*sizeof(char);
	char *key = malloc(klen);
	memcpy(key, (char *) key_buffer, klen);
	// Allocate and fill the value.
	size_t vlen = val_len*sizeof(char);
	char *val = malloc(vlen);
	memcpy(val, (char *) word_buffer, vlen);
	// Link value by the key in short2long.
	entry =  Hash_CreateEntry(&short2long, key, &flag);
	Hash_SetValue(entry, val);
	// Link key by the value in long2short
	entry =  Hash_CreateEntry(&long2short, val, &flag);
	Hash_SetValue(entry, key);
	// TEST
	char *tkey, *tval;
	// Find key in short2long
	entry = Hash_FindEntry(&short2long, key);
	tval = Hash_GetValue(entry);
	entry = Hash_FindEntry(&long2short, tval);
	tkey = Hash_GetValue(entry);
	printf("%s %s\n", tkey, tval);
}

void fill_dict() {
	FILE* fp;
	int mode = KEY;
	int ki = 0;
	int wi = 0;
	char c;
	fp = fopen("enwik8.dic", "r");
	while (EOF != (c = fgetc(fp))) {
		if (' ' == c) {
			mode = WORD;
		} else if ('\n' == c) {
			hash_put(ki, wi);
			ki = wi = 0;
			mode = KEY;
		} else if (mode == WORD) {
			word_buffer[wi++] = c;
		} else {
			key_buffer[ki++] = c;
		}
	}
}

main() {
	Hash_InitTable(&long2short, DICTIONARY_SIZE);
	Hash_InitTable(&short2long, DICTIONARY_SIZE);
	fill_dict();
	return 0;
}

