#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include "nash/sprite.h"
#include "nash/hash.h"

struct ls {
    void      *car;
    struct ls *cdr;
};
typedef struct ls ls;

struct ar {
    void   **data;
    size_t   len;
    size_t   _len;
};
typedef struct ar ar;

struct elem {
    short         sym;
    size_t        pos;
    struct elem  *next;
    struct elem  *prev;
    struct elem  *nextpos;
    struct elem  *prevpos;
    struct elem **access;
    struct elem  *tail;
};
typedef struct elem elem;

elem *quick_access[256][256];

struct context {
    char           *name;
    int             id;
    struct context *grammar[256];
    size_t          width;
};
typedef struct context context;

struct letter {
    unsigned int c;
    size_t cnt;
};

struct ngram {
    char   *word;
    size_t  order;
    size_t  count;
    int    mess;
};
typedef struct ngram ngram;

typedef struct page {
    size_t pos;
    elem *ptr;
    ar *ngrams;
    ngram *dominant;
} page;

typedef struct {
    ar *subclusters;
    Hash_Table *subgrams;
    int contains_pages;
    ngram *dominant;
} cluster;

typedef struct {
    size_t count;
    ngram *ngram;
} counter;

struct entry {
    ngram *n;
    elem *pos;
};
typedef struct entry entry;

struct pair {
    ngram *start;
    ngram *end;
    int           cycles;
    int           maxdepth;
    int           inouts;
};
typedef struct pair pair;

struct statistics {
    struct letter *letter_count;
    struct pair    pairs[256];
    size_t         npair;
    size_t         size;
    char          *buffer;
    size_t         nwords;
} doc_stat;

//FILE *errata;
FILE *rules;
context *gluzd;
size_t next_id;
elem *text;
ar *ngrams;
ar *dic;
ar *pairs;
ar *gc;
ar *pages;
Hash_Table *ngramsh;
FILE *fp, *fout, *dout;

int sign(int x) {
    return (x>0)-(x<0);
}

ls *cons(void *x, ls *rest) {
    ls *el = malloc(sizeof(ls));
    el->car = x;
    el->cdr = rest;
    return el;
}

void append(ar *a, ar *b) {
    if (a->_len <= a->len + b->len)
        a->data = realloc(a->data, (a->_len = a->len + b->len + 100) * sizeof(void *));
    int i; for (i = 0; i < b->len; i++) {
        a->data[a->len + i] = b->data[i];
    }
    a->len += b->len;
}

void ar_add(ar *a, void *ptr) {
    a->data[a->len++] = ptr;
    if (a->_len == a->len)
        a->data = realloc(a->data, ((a->_len += 100) * sizeof(void *)));
}

ar *new_ar() {
    ar *x = malloc(sizeof(ar));
    x->data = malloc(100 * sizeof(void *));
    x->len = 0;
    x->_len = 100;
    return x;
}

void free_ar(ar *ar) {
    free(ar->data);
    free(ar);
}

void ar_sort(ar *a, int (*compar)(const void *, const void *)) {
    qsort(a->data, a->len, sizeof(void *), compar);
}

void _show(context *x, int level) {
    int maxindent = 20;
    int i, j, padlen = level * 3;
    char *pad = malloc(padlen * sizeof(char));
    for (i = 0; i<padlen; i++) pad[i] = '|';
    pad[padlen-1] = 0;
    fprintf(stderr, "%s >> \"%s\" << id: %d, width: %d, '", pad, x->name, x->id, x->width);
    for (i = 0, j = 0; i < 256 && j < (88 - maxindent); i++)
        if (x->grammar[i])
            j++, fprintf(stderr, "%c", isprint(i)? i: '.');
    if (j < x->width) fprintf(stderr, "...");
    fprintf(stderr, "'\n");
    for (i = 0; i < 256; i++)
        if (x->grammar[i])
            _show(x->grammar[i], level + 1);
    free(pad);
}

void show_context(context *x) {
    _show(x, 0);
}

context *get(unsigned char c, context *x) {
    return x->grammar[c];
}

int is_last(context *x) {
    return !x->width;
}

context *new(char *name) {
    context *con = calloc(1, sizeof(context));
    con->id = next_id++;
    con->name = name;
    return con;
}

void set_grammar(context *con, const unsigned char *grammar, size_t len) {
    con->width = len;
    int i; for (i = 0; i < len; i++) {
        con->grammar[ grammar[i] ] = new(con->name);
    }
}

// this way you make a to match c with context b
void chain(context *a, unsigned char c, context *b) {
    context *old = a->grammar[c];
    if (old)
        fprintf(stderr, "*** Subcontext exists ***\n"),
            exit(1);
    else
        a->width++,
            a->grammar[c] = b;
}

context *_create(char *match, size_t level) {
    context *x = new(match);
    if (level < strlen(match))
        chain(x, match[level], _create(match, level + 1));
    return x;
}

context *match(char *match) {
    return _create(match, 0);
}

ls *find_context_filepos(context *needle, context *start, FILE *fp) {
    ls *result = NULL;
    context *current = start, *next = NULL;
    char c;
    while (EOF != (c = fgetc(fp))) {
        if (current == needle) { // match found
            long *i = malloc(sizeof(long));
            *i = ftell(fp);
            result = cons((void *) i, result);
            current = start;
        } else {
            next = get(c, current);
            if (is_last(next)) {
                current = start; // match not found, start over
            } else {
                current = next; // match in process
            }
        }
    }
    return result;
}

void explore(FILE *fp, context *x) {
    int mincount = 100;
    int i;
    size_t *count = calloc(256, sizeof(size_t));
    ls *pos = find_context_filepos(x, x, fp);
    if (pos == NULL)
        fprintf(stderr, "*** NO ENTRIES FOR THIS CONTEXT ***\n");
    while (NULL != pos) {
        int p = *((int *)pos->car);
        fseek(fp, p, SEEK_SET);
        count[fgetc(fp)]++;
        pos = pos->cdr;
    }
    rewind(fp);
    for (i = 0; i < 256; i++)
        if (count[i] > mincount) {
            //chain(x, count[i], match(""));
            fprintf(stderr, "*** '%c' (%d) %d ***\n", i, i, count[i]);
        }
}

void display(char *buf, size_t len, FILE *fout) {
    int i;
    for (i = 0; i < len; i++)
        if (buf[i] == '"') fputs("\\\"", fout);
        else if (buf[i] == '\n') fputs("\\n", fout);
        else if (buf[i] == '\\') fputs("\\\\", fout);
        else fputc(buf[i], fout);
}

void _dump(unsigned char *buf, size_t *len, FILE *fout) {
    int i;
    fputs("\nfputs(\"", fout);
    for (i = 0; i < *len; i++)
        if (buf[i] == '"') fputs("\\\"", fout);
        else if (buf[i] == '\n') fputs("\\n", fout);
        else if (buf[i] == '\\') fputs("\\\\", fout);
        else fputc(buf[i], fout);
    fputs("\", stdout);", fout);
    (*len) = 0;
}

void _add(unsigned char **buf, size_t *size, size_t *i, char c, unsigned char
        **was, unsigned char *errbuf, unsigned char *databuf, size_t errp, size_t datap, FILE *fout) {
    if (*was != *buf)
        _dump(*buf, (*buf == errbuf)? &errp: &datap, fout);
    if (*i == *size)
        (*buf) = realloc(*buf, ((*size) *= 2) * sizeof(char));
    (*buf)[(*i)++] = c;
    *was = *buf;
}

void describe(FILE *fin, FILE *fout, context *x) {
    char c;
    size_t errp = 0, datap = 0;
    size_t datasize = 1024, errsize = 1024;
    unsigned char *errbuf = malloc(errsize * sizeof(char));
    unsigned char *databuf = malloc(datasize * sizeof(char));
    unsigned char *was = errbuf;
    fputs("#include \"rules.h\"\n", fout);
    //describe_contexts(x);
    fputs("int main(){", fout);
    context *current = x, *next = NULL;
    while (EOF != (c = fgetc(fin))) {
        if (!is_last(current)) {
            next = get(c, current);
        } if (!next) {
            _add(&errbuf, &errsize, &errp, c, &was, errbuf, databuf, errp, datap, fout);
        } else if (is_last(next)) {
            _add(&databuf, &datasize, &datap, c, &was, errbuf, databuf, errp, datap, fout);
        }
        current = next;
    }
    _dump(databuf, &datap, fout);
    _dump(errbuf, &errp, fout);
    fputs("\nreturn 0;\n}\n", fout);
}

int _compar(const void *a, const void *b) {
    return ((struct letter *)a)->cnt - ((struct letter *)b)->cnt;
}

void count_letters(FILE *fp) {
    char c;
    int i = 0, initsz = 1024 * 1024;
    doc_stat.letter_count = calloc(256, sizeof(struct letter));
    doc_stat.buffer = malloc(initsz * sizeof(char));
    while (EOF != (c = fgetc(fp))) {
        doc_stat.letter_count[(unsigned char)c].cnt++;
        if (i == initsz)
            doc_stat.buffer = realloc(doc_stat.buffer, (initsz *=2) * sizeof(char));
        doc_stat.buffer[i++] = c;
    }
    qsort(doc_stat.letter_count, 256, sizeof(struct letter), &_compar);
    doc_stat.size = i;
    rewind(fp);
}

double max(int a, int b) {
    return (a > b)? a: b;
}

int calc_save(const ngram *n) {
    return n->order * n->count - n->count - n->order - 1; // save plain
    return n->order; //naive greedy
    return n->order * n->count - n->count - log(n->order) - 2; // save recursive
}

double calc_pair_compr_ratio(const pair *p) {
    int nA = p->start->order, nB = p->end->order;
    int R = p->cycles;
    return p->cycles / p->maxdepth + nA + nB;
    return ((nB+nA)*R+(nB+nA))/(R+log(nB)+log(nA)+3);;
}

double calc_compr_ratio(const ngram *n) {
    return (n->order*n->count)/(n->count+n->order+1.0); // compression ratio plain
    return (n->order*n->count)/(n->count+log(n->order)+2.0); // compression ratio recursive
    return n->order * n->count; //naive greedy
}

double calc_pattern_compr_rat(const ngram *start, const ngram *end) {
    return 1.0;
}


int min(int x, int y) {
    return (x < y)? x: y;
}

void show_ngram(ngram *n) {
    fprintf(stderr, "%3d)\t%u\t%d\t%f\t\"", n->order, n->count, calc_save(n), calc_compr_ratio(n));
    display(n->word, strlen(n->word), stderr);
    fprintf(stderr, "\"\n");
}

void show_pairs(size_t len) {
    len = ((!len || pairs->len < len)? pairs->len: len);
    int i; for (i = 0; i < len; i++) {
        pair *p = pairs->data[i];
        show_ngram(p->start), show_ngram(p->end);
        fprintf(stderr, "*** >> %d %d %d (%f) << ***\n"
                "------------------------------\n",
                p->cycles, p->inouts, p->maxdepth, calc_pair_compr_ratio(p));
    }
}

void show_dic(int order, size_t len) {
    len = ((!len || dic->len < len)? dic->len: len);
    int i; for (i = 0; i < len; i++) {
        ngram *n = dic->data[i];
        if (!order || n->order == order) {
            show_ngram(n);
        }
    }
}

void show_ngrams(int order, size_t len) {
    len = ((!len || ngrams->len < len)? ngrams->len: len);
    int i; for (i = 0; i < len; i++) {
        ngram *n = ngrams->data[i];
        if (!order || n->order == order) {
            show_ngram(n);
        }
    }
}

int _number_of_smaller_ngrams_that_remain(int size, int field, char *buf, Hash_Table *pool) {
    //fprintf(stderr, "*** sz: %d \t fld: %d \t ngr: %s ***\n", size, field, buf);
    Hash_Entry *e;
    int result = 200; // enough
    int i;
    if (size) {
        for (i = size; i <= field; i++) {
            //fprintf(stderr, "*** %s ***", buf);
            e = Hash_FindEntry(pool, buf + i - size, size);
            if (NULL == e) {
                return 0;
            } else {
                int remains = *((int *)Hash_GetValue(e));
                if (result > remains) {
                    result = remains;
                }
            }
        }
    }
    //fprintf(stderr, "*** %d ***\n", result);
    return result;
}

ngram *new_ngram(char *strptr, size_t len, size_t count) {
    ngram *n = malloc(sizeof(ngram));
    n->order = len;
    n->word = strptr;
    n->count = count;
    n->mess = 0;
    return n;
}

void free_ngram(ngram *n) {
    free(n->word);
    free(n);
}

int rev_strcmp(char *a, int pa, char *b, int pb) {
    while (1) {
        --pa, --pb;
        if (pa < 0 && pb < 0) return 0;
        if (pa < 0 || (pb >= 0 && (a[pa] < b[pb]))) return -1;
        if (pb < 0 || (a[pa] > b[pb])) return 1;
    }
}

int _entry_compar_pos(const void *a, const void *b) {
    entry *bb = (*((entry **)b)), *aa = (*((entry **)a));
    return aa->pos->pos - bb->pos->pos;
}

int _ngram_alphabetic_compar(const void *a, const void *b) {
    ngram *bb = (*((ngram **)b)), *aa = (*((ngram **)a));
    return strcmp(aa->word, bb->word);
}

int _ngram_compar_rev(const void *a, const void *b) {
    ngram *bb = (*((ngram **)b)), *aa = (*((ngram **)a));
    return rev_strcmp(bb->word, strlen(bb->word), aa->word, strlen(aa->word)) * 2
        + sign(bb->order - aa->order) + sign(bb->count - aa->count) * 4;
}

int _pair_crat_compar(const void *a, const void *b) {
    pair *bb = (*((pair **)b)), *aa = (*((pair **)a));
    return (calc_pair_compr_ratio(bb) - calc_pair_compr_ratio(aa)) * 1000;
}

int _ngram_cratio_compar(const void *a, const void *b) {
    ngram *bb = (*((ngram **)b)), *aa = (*((ngram **)a));
    return (calc_compr_ratio(bb) - calc_compr_ratio(aa)) * 1000;
}

int _ngram_npages_compar1(const void *a, const void *b) {
    ngram *bb = (*((ngram **)b)), *aa = (*((ngram **)a));
    return aa->mess - bb->mess;
}

int _ngram_npages_compar2(const void *a, const void *b) {
    ngram *bb = (*((ngram **)b)), *aa = (*((ngram **)a));
    return bb->mess - aa->mess;
}

int pagecmp(ar *a, ar *b, int idx) {
    if (idx == a->len || idx == b->len)
        return a->len - b->len;
    else {
        ngram *na = a->data[idx];
        ngram *nb = b->data[idx];
        if (na != nb)
            return sign(na->mess - nb->mess) * 2 + sign((int)na - (int)nb);
        else return pagecmp(a, b, idx + 1);
    }
}

int _page_ngram_compar1(const void *a, const void *b) {
    page *bb = (*((page **)b)), *aa = (*((page **)a));
    return pagecmp(aa->ngrams, bb->ngrams, 0);
}

int _page_ngram_compar2(const void *a, const void *b) {
    page *bb = (*((page **)b)), *aa = (*((page **)a));
    return pagecmp(bb->ngrams, aa->ngrams, 0);
}

int _ngram_save_compar(const void *a, const void *b) {
    ngram *bb = (*((ngram **)b)), *aa = (*((ngram **)a));
    return (calc_save(bb) - calc_save(aa)) * 1000;
}

int _ngram_compar(const void *a, const void *b) {
    ngram *bb = (*((ngram **)b)), *aa = (*((ngram **)a));
    return strcmp(bb->word, aa->word) * 2
        + sign(bb->order - aa->order) + sign(bb->count - aa->count) * 4;
}

int _ngram_compar_ord(const void *a, const void *b) {
    ngram *bb = (*((ngram **)b)), *aa = (*((ngram **)a));
    return sign(bb->order - aa->order) + sign(bb->count - aa->count) * 4;
}

int _ngram_compar_count(const void *a, const void *b) {
    ngram *bb = (*((ngram **)b)), *aa = (*((ngram **)a));
    if (bb->order == aa->order)
        return strncmp(bb->word, aa->word, aa->order) + sign(bb->count - aa->count) * 4;
    else
        return sign(bb->order - aa->order) + sign(bb->count - aa->count) * 4;
}

void _remove_bogus(size_t start) {
    int i, k;
    ngram **ar = (ngram **)ngrams->data, *a, *b;
    a = ar[i = k = start];
    while (i+1 < ngrams->len) {
        b = ar[++i];
        if (a->count != b->count || (strstr(a->word, b->word) == NULL && strstr(a->word, b->word) == NULL)) {
            ar[k++] = a;
            a = b;
        } else if (strstr(a->word, b->word) != NULL) {
            free(b);
        } else {
            free(a);
            a = b;
        }
    }
    ar[k++] = a;
    ngrams->len = k;
}

void remove_bogus(size_t start) {
    qsort(ngrams->data + start, ngrams->len - start, sizeof(void *), _ngram_compar_rev);
    _remove_bogus(start);
    qsort(ngrams->data + start, ngrams->len - start, sizeof(void *), _ngram_compar);
    _remove_bogus(start);
    qsort(ngrams->data + start, ngrams->len - start, sizeof(void *), _ngram_compar_ord);
    _remove_bogus(start);
}

void show_table(int minrepeat, Hash_Table *t) {
    Hash_Search ptr;
    Hash_Entry *e;
    for (e = Hash_EnumFirst(t, &ptr); e != NULL; e = Hash_EnumNext(&ptr)) {
        int *counter = (int *)Hash_GetValue(e);
        if (*counter >= minrepeat) {
            fprintf(stderr, "%d\t***", *counter);
            display(e->nameptr, e->namelen, stderr);
            fprintf(stderr, "***\n");
        }
    }
}

void count_ngrams(FILE *fp, int minrep) {
    int fr = 1, to = 1, pos= 0, j = 0, is_new, factor=-log(doc_stat.size);
    char *buf;
    ngrams = new_ar();
    ngramsh = malloc(sizeof(Hash_Table));
    Hash_InitTable(ngramsh, 1024);
    Hash_Table *pool = malloc(sizeof(Hash_Table));
    Hash_InitTable(pool, 1024 * 1024);
    Hash_Table *new_entries = malloc(sizeof(Hash_Table));
    Hash_InitTable(new_entries, 1024 * 1024);
    Hash_Entry *e;
    while (1) {
        fprintf(stderr, "*** %d %d ***\n", fr, to);
        for (pos = 0; pos < doc_stat.size - fr + 1; pos++) {
            buf  = doc_stat.buffer + pos;
            for (j = fr-1; j < to && (j+pos <= doc_stat.size-j || pos >= j) && j < (doc_stat.size - pos); ) {
                ++j;
                int left = _number_of_smaller_ngrams_that_remain(fr - 1, j, buf, pool);
                if (left >= minrep || (((e = Hash_FindEntry(new_entries, buf, j)) != NULL) &&
                            ((*(int *)Hash_GetValue(e))+left >= minrep))) {
                    e = Hash_CreateEntry(new_entries, buf, j, &is_new);
                    if (is_new) {
                        int *cnt = malloc(sizeof(int));
                        *cnt = 1;
                        Hash_SetValue(e, cnt);
                        //fprintf(stderr, "*** %s %d ***\n", buf, *((int *)Hash_GetValue(e)));
                    } else {
                        ++(*((int *)Hash_GetValue(e)));
                    }
                } else {
                    break; // this line gives sense to all the rest
                }
            }
            if ((doc_stat.size - pos) > (fr - 2)) {
                e = Hash_FindEntry(pool, buf, fr-1);
                if (e != NULL && 0 == --(*((int *)Hash_GetValue(e)))) {
                    //fprintf(stderr, "decr %s ***\n", buf);
                    free(Hash_GetValue(e));
                    Hash_DeleteEntry(pool, e);
                }
            } else {
                //fprintf(stderr, "*** HERE ***\n");
            }
        }
        Hash_Search ptr;
        for (e = Hash_EnumFirst(pool, &ptr); e != NULL; e = Hash_EnumNext(&ptr))
            free(Hash_GetValue(e));
        Hash_DeleteTable(pool);
        Hash_InitTable(pool, new_entries->numEntries);
        size_t oldsz = ngrams->len;
        for (e = Hash_EnumFirst(new_entries, &ptr); e != NULL; e = Hash_EnumNext(&ptr)) {
            int *counter = (int *)Hash_GetValue(e);
            if (*counter >= minrep) {
                //fprintf(stderr, "*** %s\t%d ***\n", e->name, *counter);
                ngram *n = new_ngram(e->nameptr, e->namelen, *counter); //TODO: link e->name instead of alloc. a word.
                ar_add(ngrams, n);
                //Hash_SetValue(Hash_CreateEntry(ngramsh, e->name, NULL), n);
                if (n->order == to) {
                    e = Hash_CreateEntry(pool, e->nameptr, e->namelen, NULL);
                    Hash_SetValue(e, counter);
                }
            }
        }
        //remove_bogus(oldsz);
        fprintf(stderr, "*** NEW: %d -- %d ***\n", new_entries->numEntries, ngrams->len - oldsz);
        Hash_DeleteTable(new_entries);
        Hash_InitTable(new_entries, ngrams->len - oldsz);
        // after 6, we grow block exponentially
        fr = to + 1;
        if (factor < 1) {
            ++factor;
            to = to + 1;
        } else to += (factor *=2);
        if (pool->numEntries == 0) {
            Hash_DeleteTable(pool);
            free(pool);
            Hash_DeleteTable(new_entries);
            free(new_entries);
            return;
        }
    }
}

elem *mktext(char *buf, size_t len) {
    elem *text = malloc(len * sizeof(elem));
    elem *prev = &text[0];
    prev->sym     = buf[0];
    prev->pos     = 0;
    prev->prev = prev->nextpos = prev->prevpos = prev->tail = NULL;
    elem *e;
    int i; for (i = 1; i < len; i++) {
        e          = &text[i];
        e->sym     = buf[i];
        e->pos     = i;
        e->prev    = prev;
        e->next = e->nextpos = e->prevpos = e->tail = NULL;
        e->access = NULL;
        prev->next = e;
        // link to quick_access: if no link yet, make it; otherwise tail.
        elem **accessor  = &(quick_access[(unsigned char)prev->sym][(unsigned char)e->sym]);
        if (*accessor == NULL) {
            prev->access = accessor;
            *accessor = prev->tail = prev;
        } else {
            prev->access = NULL;
            elem *first = *accessor,
                 *last = first->tail;
            first->tail = last->nextpos = prev;
            prev->prevpos = last;
        }
        prev = e;
    }
    // for (i = 0; i < len; i++) {
    //     fprintf(stderr, "*** '%c' %d ***\n", text[i].sym, text[i].pos);
    // }
    return text;
}

int _compr2_match(ngram *n, elem *match) {
    int j; for (j = 0; j < n->order; j++) {
        if (match == NULL || match->sym != n->word[j]) {
            return 0;
        }
        match = match->next;
    }
    return 1;
}

elem *next_entry(ngram *n, elem *current_entry) {
    elem *pos = (current_entry == NULL)?
        quick_access[(unsigned char)n->word[0]][(unsigned char)n->word[1]]
        :
        current_entry->nextpos;
    while (pos != NULL) {
        if (_compr2_match(n, pos))
            return pos;
        pos = pos->nextpos;
    }
    return NULL;
}

elem *mkelem(int sym, size_t pos, elem *prev, elem *next, elem *prevpos, elem *nextpos, elem **access) {
    elem *e = malloc(sizeof(elem));
    e->sym = sym;
    e->pos = pos;
    e->next = next;
    e->prev = prev;
    e->nextpos = nextpos;
    e->prevpos = prevpos;
    e->access = access;
    return e;
}

void text2buf(elem *start) {
    int i = 0;
    while (NULL != start) {
        doc_stat.buffer[i++] = ((start->sym < 255)? start->sym: '^');
        start = start->next;
    }
    doc_stat.size = i;
}

void textPrint(elem *start, FILE *fout) {
    while (NULL != start) {
        fputc(((start->sym < 255)? start->sym: '^'), fout);
        start = start->next;
    }
}

elem *mkdicref(int level, elem *prev, elem *next) {
    elem *first = prev = mkelem(256 + level, 0, prev, next, NULL, NULL, NULL);
    ar_add(gc, first);
    first->tail = prev;
    return first;
}

/*
elem *mkdicref(int level, elem *prev, elem *next) {
    int bod = ((level<256)? 251: ((level < 256 * 256)? 252: 253));
    elem *first = prev = mkelem(bod, 0, prev, next, NULL, NULL, NULL);
    ar_add(gc, first);
    while (level > 0) {
        prev = prev->next = mkelem(level % 256, 0, prev, next, NULL, NULL, NULL);
        ar_add(gc, prev);
        level /=256;
    }
    first->tail = prev;
    return first;
}
*/

void _substract_use(char *str, size_t len, int ucount) {
    //display(str, strlen(str), stderr);
    Hash_Entry *e = Hash_FindEntry(ngramsh, str, len);
    ngram *n = Hash_GetValue(e);
    if (n->count < ucount) abort();
    n->count -= ucount;
}

void recalc_counts(ngram *n) {
    char *buf = n->word;
    int i; for (i = 0; i < n->order; i++) {
        char *pos = buf + i;
        _substract_use(pos, n->order - i, n->count);
        int j; for (j = i+1; j < n->order; j++) {
            buf[j] = 0;
            _substract_use(pos, n->order - j, n->count);
        }
    }
}

void clean_ngrams() {
    int i, j = 0;
    for (i = 0; i < ngrams->len; i++) {
        ngram *nn = ngrams->data[i];
        if (nn->count == 0) {
            free_ngram(nn);
        } else {
            ngrams->data[j++] = nn;
        }
    }
    ngrams->len = j;
}

int loglog(int N) {
    return exp(log(N)/(1+log(N)/(1+log(N)))) * 2;
    return exp(log(N)/(1+log(N)/(1+log(N))));
}

void compr2(elem **start) {
    dic = new_ar();
    int orig_size = doc_stat.size;
    int level = 0;
    int mess_depth = 0, mess_end = ngrams->len; // estimated stripe of unsorted ngrams.
    //size_t comprlen = 0;
    //int i; for (i = 0; i < ngrams->len; i++) {
    ar *matches = new_ar();
    int cursor = 0, cursor_backup = 0, dryrun = 0;
    ngram *n = ngrams->data[cursor++];
    while (cursor < ngrams->len && calc_compr_ratio(n) > 1) {
        //show_ngram(n);
        elem *pos = quick_access[(unsigned char)n->word[0]][(unsigned char)n->word[1]];
        while (pos != NULL) {
            //fprintf(stderr, "check: %d: '%c' ***\n", pos->pos, pos->sym);
            if (_compr2_match(n, pos)) {
                //fprintf(stderr, "MATCH: %d ***\n", pos->pos);
                ar_add(matches, pos);
            }
            pos = pos->nextpos;
        }
       //fprintf(stderr, "*** %d %d ***\n", n->count, matches->len);
        if (matches->len > n->count) {
            abort(); // WTF?
        } else if (!dryrun && matches->len == n->count) {
            fputc('.',stderr);
            //show_ngram(n);
            int i; for (i = 0; i < matches->len; i++) {
                pos = matches->data[i];
                elem *sign = pos;
                int j;
                for (j = 0; j < n->order; j++) {                  // == REMOVE FAST ACCESS ==
                    if (sign->access == NULL &&
                            sign->prevpos != NULL) {              // A position in a chain;
                       //fprintf(stderr, "-%c", sign->sym);        // relink chain w/o element.
                        sign->prevpos->nextpos = sign->nextpos;
                        if (sign->nextpos != NULL)
                            sign->nextpos->prevpos =
                                sign->prevpos;
                    } else if (sign->access == NULL) {            // An unlinked element; no action
                       //fprintf(stderr, ".%c", sign->sym);
                    } else if (sign->nextpos) {                   // First in its chain.
                       //fprintf(stderr, "^%c", sign->sym);        // NB. ->access exists at this point.
                        (*sign->access) = sign->nextpos;          // Link the next element in the
                        sign->nextpos->access = sign->access;     // chain in place of this one.
                        sign->nextpos->prevpos = NULL;
                    } else {                                      // single position in
                       //fprintf(stderr, "#%c", sign->sym);        // the chain.
                        (*sign->access) = NULL;                   // Remove the chain.
                    }
                    sign = sign->next;
                }
                if (sign != NULL) {
                   //fprintf(stderr, " |-> '%c' ***\n", sign->sym);
                } else {
                   //fprintf(stderr, " |-> EOF ***\n");
                }
                elem *dicref = mkdicref(dic->len, pos->prev, sign);
                if (pos->prev != NULL)
                    pos->prev->next = dicref;
                else *start = dicref;
                if (sign != NULL)
                    sign->prev = dicref->tail;
               //fprintf(stderr, "\n----------------\n");
                doc_stat.size = doc_stat.size - n->order + 1;
            }
            ar_add(dic, new_ngram(n->word, n->order, n->count));
            doc_stat.nwords += n->count;
            //recalc_counts(n);
        }
        int xflag = 0;
        if (dryrun && (cursor == ngrams->len - 1 || matches->len == 0 || matches->len == n->count)) { // TODO move up
            cursor = cursor_backup;
            dryrun = 0;
            xflag = 1;
            ++mess_depth;
        } else if (matches->len == 0 || matches->len == n->count) { // TODO move up
            if (n->mess) {
                --mess_depth;
               //fprintf(stderr, "*** -- %d - %d (%d) ***\n", cursor, mess_end - mess_depth, mess_depth);
            }
            n->count = 0;
            ++cursor;
            int mess_start = mess_end - mess_depth;
            if (mess_depth && cursor == mess_start) {
               //fprintf(stderr, "***  ! %d - %d (%d) ***\n", cursor, mess_start, mess_depth);
                xflag = 1;
                //fputc('!',stderr);
            } else if (mess_depth && cursor > mess_start) {
                   //fprintf(stderr, "*** WATCH YOUR CURSOR! ***\n");
            } else {
                xflag = 0;
               //fprintf(stderr, "***  | %d - %d (%d) ***\n", cursor, mess_start, mess_depth);
            }
        } else {
            if (!dryrun) {
                //dryrun = 1;
                cursor_backup = cursor;
            }
            n->count = matches->len;
            n->mess = 1;
            ++mess_depth;
            //fprintf(stderr, "*** ++ %d - %d (%d) ***\n", cursor, mess_end - mess_depth, mess_depth);
            xflag = 1;
            //++cursor;
        }
        if (xflag) {
            int len = ngrams->len - cursor;
            if (mess_depth > loglog(len)) {
                int i; for (i = cursor; i < mess_end; i++) {
                    ((ngram *)ngrams->data[i])->mess = 0;
                }
                qsort(ngrams->data + cursor, len, sizeof(void *), _ngram_cratio_compar);
                mess_end = ngrams->len;
                mess_depth = 0;
                //fputc(':',stderr);
            } else {
                qsort(ngrams->data + cursor, min(mess_depth + loglog(len) / 10, len), sizeof(void *), _ngram_cratio_compar);
                mess_end = cursor + mess_depth + loglog(len) / 10;
            }
           //fprintf(stderr, "*** SORT %d - %d (%d) ***\n", cursor, cursor + min(mess_depth * 2, len), mess_depth);
            xflag = 0;
        }
        matches->len = 0;
        ++level;
        n = ngrams->data[cursor];
    }
    fprintf(stderr, "\n================ %d) WAS: %d NOW: %d (%d) RATIO: %d "
            "==================\n", level, orig_size, doc_stat.size, dic->len, orig_size / doc_stat.size);
}

ngram *joined(ngram *a, ngram *b) {
    char *abword = malloc((a->order + b->order + 1) * sizeof(char));
    strcpy(abword, b->word);
    strcat(abword, a->word);
    Hash_Entry *e = Hash_FindEntry(ngramsh, abword, strlen(abword));
    free(abword);
    return (e == NULL)? NULL: Hash_GetValue(e);
}

entry *new_entry(ngram *n, elem *el) {
    entry *en = malloc(sizeof(entry));
    en->n = n, en->pos = el;
    return en;
}

void explore_cycles(ngram *start, ngram *end, ar *entries, int *position, int depth, int *maxdepth, int *cycles, int *inouts) {
    int outpos = 0;
    *maxdepth = (*maxdepth > depth)? *maxdepth: depth;
    entry *prev = entries->data[*position];
    ++(*position);
    //fprintf(stderr, "*** %d\n", depth);
    while (*position < entries->len) {
        entry *en = entries->data[*position];
        if (en->n == start) {
            if (en->pos->pos < prev->pos->pos + end->order)
                ++(*position);
            else {
                if (outpos) {
                    int dist = en->pos->pos - outpos - end->order;
                    //fprintf(stderr, "*** >%d< ***\n", dist);
                    assert(dist >= 0);
                    if (dist < 4) ++(*cycles);
                }
                explore_cycles(start, end, entries, position, depth+1, maxdepth, cycles, inouts);
                if (*position == entries->len) return;
                entry *exit = entries->data[*position];
                outpos = exit->pos->pos;
                prev = entries->data[*position];
                ++(*position);
            }
        } else if (en->n == end) {
            if (en->pos->pos < prev->pos->pos + start->order)
                ++(*position);
            else {
                ++(*inouts);
                return;
            }
        } else {
            abort(); // a wrong ngram in entries list
        }
    }
}

pair *new_pair(ngram *a, ngram *b, int cycles, int maxdepth, int inouts) {
    pair *p = malloc(sizeof(pair));
    p->start=a, p->end=b, p->cycles=cycles, p->maxdepth=maxdepth, p->inouts=inouts;
    return p;
}

void templatize(ngram *a, ngram *b) {
    ngram *ab = joined(a, b);
    if (ab != NULL) {
        ar *entries = new_ar();
        elem *el;
        for (el = next_entry(a, NULL); el != NULL; el = next_entry(a, el))
            ar_add(entries, new_entry(a, el));
        for (el = next_entry(b, NULL); el != NULL; el = next_entry(b, el))
            ar_add(entries, new_entry(b, el));
        ar_sort(entries, _entry_compar_pos);
        int pos = 0;
        int cycles = 0;
        int depth = 0;
        int maxdepth = 0;
        int inouts = 0;
        explore_cycles(a, b, entries, &pos, depth, &maxdepth, &cycles, &inouts);
        if (maxdepth < cycles)
            ar_add(pairs, new_pair(a, b, cycles, maxdepth, inouts));
    }
}

void struct_explore(FILE *fp) {
    count_letters(fp);
    int minrep = 2;
    //remove_bogus(0);
    count_ngrams(fp, minrep);
    fprintf(stderr, "*** size: %d ngrams: %d ***\n", doc_stat.size, ngrams->len);
    ar_sort(ngrams, _ngram_cratio_compar);
    //show_ngrams(0, 0);
    pairs = new_ar();
    //int i; for (i = 0; i < ngrams->len; i++) {
    //    int j; for (j = 0; j < ngrams->len; j++) {
    //        ngram *a = ngrams->data[i];
    //        ngram *b = ngrams->data[j];
    //        templatize(b, a);
    //    }
    //}
    //ar_sort(pairs, _pair_crat_compar);
    //show_pairs(7);
    //show_ngrams(0, 1);
}

int determine_chunksize(size_t L, size_t Sigma) {
    double guess = 1;
    while (guess != (int)((L*log(L)+(1-log(guess))*L)/(L-Sigma) + 0.5)) {
        //fprintf(stderr, "*** %d ***\n", (int)(L*log(L)+(1-log(guess))*L)/(L-Sigma));
        ++guess;
    }
    return guess;
}

page *new_page(size_t pos, elem *ptr, ar *ngrams) {
    page *p = malloc(sizeof(page));
    p->pos = pos;
    p->ptr = ptr;
    p->ngrams = ngrams;
    return p;
}

cluster *new_cluster(ngram *dominant, ar *subclusters, size_t htlen, int contains_pages) {
    cluster *c = malloc(sizeof(cluster));
    c->dominant = dominant;
    c->subclusters = subclusters;
    c->contains_pages = contains_pages;
    c->subgrams = malloc(sizeof(Hash_Table));
    Hash_InitTable(c->subgrams, htlen);
    return c;
}

void text2pages(elem *text) {
    int pos = 0;
    elem *pgstart = text;
    //int chunksize = determine_chunksize(doc_stat.nwords, dic->len);
    int chunksize = 256;
    pages = new_ar();
    Hash_Table uwords;
    Hash_InitTable(&uwords, chunksize);
    page *pg = new_page(pos++, pgstart, new_ar());
    while (text != NULL) {
        if (text->sym > 255) {
            ngram *n = dic->data[text->sym - 256];
            Hash_Entry *e = Hash_FindEntry(&uwords, n->word, n->order);
            if (e != NULL) {
                ++(*(int *)Hash_GetValue(e));
                text = text->next;
            } else if (uwords.numEntries == chunksize) {
                Hash_DeleteTable(&uwords);
                Hash_InitTable(&uwords, chunksize);
                ar_add(pages, pg);
                pg = new_page(pos++, pgstart, new_ar());
                if (pgstart->prev != NULL)
                    pgstart->prev->next = NULL;
            } else {
                ++n->mess;
                ar_add(pg->ngrams, n);
                pgstart = text = text->next; // pgstart will be used if numEntries crosses the line.
                e = Hash_CreateEntry(&uwords, n->word, n->order, NULL);
                int *counter = malloc(sizeof(int));
                ar_add(gc, counter);
                *counter = 1;
                Hash_SetValue(e, counter);
            }
        } else {
            text = text->next;
        }
    }
    Hash_DeleteTable(&uwords);
    ar_add(pages, pg);
    fprintf(stderr, "*** %d pages by %d urefs add %f bytes ***\n", pages->len, chunksize, pages->len * log(pages->len) / 8);
}

counter *new_counter(size_t count, ngram *n) {
    counter *c = malloc(sizeof(counter));
    c->count = count;
    c->ngram = n;
    return c;
}


void _increase_hash_cnt(Hash_Table *ht, ngram *n) {
    int is_new;
    Hash_Entry *e = Hash_CreateEntry(ht, n->word, n->order, &is_new);
    if (is_new) {
        counter *c = new_counter(1, n);
        Hash_SetValue(e, c);
    } else {
        counter *c = Hash_GetValue(e);
        ++c->count;
    }
}

cluster *cluster_pages(ar *clusters, int contains_pages) {
    if (clusters->len == 1) return clusters->data[0];
    Hash_Table ngrams;
    Hash_InitTable(&ngrams, dic->len);
    ar *percluster = new_ar();
    Hash_Entry *e; Hash_Search ptr;
    int i; for (i = 0; i < clusters->len; i++) {
        Hash_Table *ht = malloc(sizeof(Hash_Table));
        ar_add(percluster, ht);
        Hash_InitTable(ht, 100);
        if (contains_pages) {
            page *p = clusters->data[i];
            elem *el = p->ptr;
            while (el != NULL) {
                if (el->sym > 255) {
                    ngram *n = dic->data[el->sym - 256];
                    _increase_hash_cnt(&ngrams, n);
                    _increase_hash_cnt(ht, n);
                }
                el = el->next;
            }
        } else {
            cluster *cl = clusters->data[i];
            for (e = Hash_EnumFirst(cl->subgrams, &ptr); e != NULL; e = Hash_EnumNext(&ptr)) {
                ngram *n = Hash_GetValue(e);
                _increase_hash_cnt(&ngrams, n);
                _increase_hash_cnt(ht, n);
            }
        }
    }
    Hash_Table groups;
    Hash_InitTable(&groups, 100);
    for (i = 0; i < percluster->len; i++) {
        Hash_Table *ht = percluster->data[i];
        double bestrate = 0.0;
        ngram *best = NULL;
        for (e = Hash_EnumFirst(ht, &ptr); e != NULL; e = Hash_EnumNext(&ptr)) {
            counter *c = Hash_GetValue(e);
            Hash_Entry *ee = Hash_FindEntry(&ngrams, c->ngram->word, c->ngram->order);
            counter *cc = Hash_GetValue(ee);
            if (cc->count > 1) {
                double tf_idf = (double)c->count / (double)cc->count;
                if (tf_idf > bestrate) {
                    bestrate = tf_idf;
                    best = c->ngram;
                }
            }
            free(c);
        }
        Hash_DeleteTable(ht);
        int is_new;
        e = Hash_CreateEntry(&groups, best->word, best->order, &is_new);
        cluster *clust;
        if (is_new) {
            clust = new_cluster(best, new_ar(), 10, contains_pages);
            Hash_SetValue(e, clust);
        } else {
            clust = Hash_GetValue(e);
        }
        ar_add(clust->subclusters, clusters->data[i]);
        if (contains_pages) {
            page *p = clusters->data[i];
            ar *subgrams = p->ngrams;
            int j; for (j = 0; j < subgrams->len; j++) {
                ngram *n = subgrams->data[j];
                e = Hash_CreateEntry(clust->subgrams, n->word, n->order, &is_new);
                if (is_new)
                    Hash_SetValue(e, n);
            }
        } else {
            cluster *cl = clusters->data[i];
            for (e = Hash_EnumFirst(cl->subgrams, &ptr); e != NULL; e = Hash_EnumNext(&ptr)) {
                ngram *n = Hash_GetValue(e);
                Hash_Entry *ee = Hash_CreateEntry(clust->subgrams, n->word, n->order, &is_new);
                if (is_new)
                    Hash_SetValue(ee, n);
            }
        }
    }
    for (e = Hash_EnumFirst(&ngrams, &ptr); e != NULL; e = Hash_EnumNext(&ptr)) {
        free(Hash_GetValue(e));
    }
    Hash_DeleteTable(&ngrams);
    free_ar(percluster);
    ar *subclusters = new_ar();
    for (e = Hash_EnumFirst(&groups, &ptr); e != NULL; e = Hash_EnumNext(&ptr)) {
        ar_add(subclusters, Hash_GetValue(e));
    }
    Hash_DeleteTable(&groups);
    free_ar(clusters);
    return cluster_pages(subclusters, 0);
}

ar *cluster2ar(ar *a, cluster *c) {
    int i;
    if (c->contains_pages) for (i = 0; i < c->subclusters->len; i++) {
        ar_add(a, c->subclusters->data[i]);
    } else for (i = 0; i < c->subclusters->len; i++) {
        cluster2ar(a, c->subclusters->data[i]);
    }
    return a;
}

void sort_pages1() {
    int i; for (i = 0; i < pages->len; i++) {
        ar_sort(((page *)pages->data[i])->ngrams, _ngram_npages_compar1);
    }
    ar_sort(pages, _page_ngram_compar1);
}

void sort_pages2() {
    int i; for (i = 0; i < pages->len; i++) {
        ar_sort(((page *)pages->data[i])->ngrams, _ngram_npages_compar1);
    }
    ar_sort(pages, _page_ngram_compar2);
}

void sort_pages3() {
    int i; for (i = 0; i < pages->len; i++) {
        ar_sort(((page *)pages->data[i])->ngrams, _ngram_npages_compar2);
    }
    ar_sort(pages, _page_ngram_compar1);
}

void sort_pages4() {
    int i; for (i = 0; i < pages->len; i++) {
        ar_sort(((page *)pages->data[i])->ngrams, _ngram_npages_compar2);
    }
    ar_sort(pages, _page_ngram_compar2);
}

void show_pages() {
    int i; for (i = 0; i < pages->len; i++) {
        page *p = pages->data[i];
        fprintf(stderr, "*** page # %d ***\n", p->pos);
        int j; for (j = 0; j < p->ngrams->len; j++) {
            ngram *n = p->ngrams->data[j];
            fprintf(stderr, "%d ", n->mess);
            //fprintf(stderr, "%p ", n);
        }
        fprintf(stderr, "\n");
        elem *el = p->ptr;
        while (el != NULL) {
            fputc(((el->sym < 255)? el->sym: '^'), stderr);
            el = el->next;
        }
        fprintf(stderr, "\n");
    }
}

void _update_model(ar *model, Hash_Table *ht) {
    Hash_Search ptr;
    Hash_Entry *e;
    model->len = 0;
    for (e = Hash_EnumFirst(ht, &ptr); e != NULL; e = Hash_EnumNext(&ptr)) {
        ar_add(model, Hash_GetValue(e));
    }
    ar_sort(model, _ngram_compar_count);
    int i; for (i = 0; i < model->len; i++) {
        ngram *n = model->data[i];
        if (i < 255) {
            n->mess = i;
        } else {
            Hash_DeleteEntry(ht, Hash_FindEntry(ht, n->word, n->order));
        }
    }
    if (model->len > 256)
        model->len = 256;
}

void encode_pages() {
    double cost = 0;
    int count = 0;
    int maxwidth = 0;
    Hash_Table dihash;
    Hash_InitTable(&dihash, dic->len);
    ar *model = new_ar();
    int i; for (i = 0; i < pages->len; i++) {
        page *pg = pages->data[i];
        elem *el = pg->ptr;
        int junk_mode = 0;
        while (el != NULL) {
            if (el->sym > 255) {
                if (junk_mode) {
                    junk_mode = 0;
                    fputc('"', fout);
                }
                ++count;
                ngram *n = dic->data[el->sym - 256];
                int is_new = 0;
                Hash_Entry *e = Hash_CreateEntry(&dihash, n->word, n->order, &is_new);
                if (is_new) {
                    fputc('\'', fout);
                    int j; for (j = 0; j < n->order; j++) {
                        fputc(n->word[j], fout);
                    }
                    fprintf(fout, "'%x'", --n->count);
                    Hash_SetValue(e, n);
                    _update_model(model, &dihash);
                } else {
                    cost += log(dihash.numEntries);
                    maxwidth = ((maxwidth < n->mess)? n->mess: maxwidth);
                    //fprintf(stderr, "*** DICREF PG: %d DICSZ: %d ***\n", pg->pos, dihash.numEntries);
                    if (--n->count == 0) {
                        Hash_DeleteEntry(&dihash, e);
                    }
                    fputc(n->mess, fout);
                    _update_model(model, &dihash);
                }
            } else {
                if (!junk_mode) {
                    junk_mode = 1;
                    //fputc('"', fout);
                }
                fputc(el->sym, fout);
            }
            el = el->next;
        }
    }
    //show_pages();
    fprintf(stderr, "*** REFS: %d FULL: %f COST: %f MAXWIDTH: %d ***\n", count, log(count)*count/8.0, cost/8.0, maxwidth);
}

void compr_rec(FILE *fp) {
    gc = new_ar();
    count_letters(fp);
    int minrep = 2;
    count_ngrams(fp, minrep);
    ar_sort(ngrams, _ngram_cratio_compar);
    //show_ngrams(0, 100);
    text = mktext(doc_stat.buffer, doc_stat.size);
    fprintf(stderr, "*** size: %d ngrams: %d ***\n", doc_stat.size, ngrams->len);
    compr2(&text);
    text2pages(text);
    encode_pages();
    //sort_pages1();
    //encode_pages();
    //sort_pages2();
    //encode_pages();
    //pages = cluster2ar(new_ar(), cluster_pages(pages, 1));
    //encode_pages();
    //sort_pages3();
    //encode_pages();
    //sort_pages4();
    //encode_pages();
}

void gcollect() {
    free(text);
    int i; for (i = 0; i < gc->len; i++) {
        free(gc->data[i]);
    }
    gc->len = 0;
}

void dic2buf() {
    int k = 0;
    int i; for (i = 0; i < dic->len; i++) {
        ngram *n = dic->data[i];
        int j; for (j = 0; j < n->order; j++) {
            doc_stat.buffer[k++] = n->word[j];
        }
        doc_stat.buffer[k++] = 0;
    }
    doc_stat.size = k;
}

void dicPrint(FILE *fout) {
    int i; for (i = 0; i < dic->len; i++) {
        ngram *n = dic->data[i];
        int j; for (j = 0; j < n->order; j++) {
            fputc(n->word[j], fout);
        }
        fputc(0, fout);
    }
}

int main () {
    next_id =  1e8;
    fp = fopen("2k", "r");
    fout = fopen("rules.c", "w");
    dout = fopen("rules.h", "w");
    //struct_explore(fp);
    compr_rec(fp);
    //show_dic(0, 0);
    //
    //dicPrint(dout);
    //textPrint(text, fout);
    //
    //fwrite(doc_stat.buffer, 1, doc_stat.size, fout);
    //explore(fp,new("empty"));
    //struct_explore(fp);
    //check(c, x);
    //context *x = match("<");
    //describe(fp, fout, x);
    fprintf(stderr, "\n----------------------------------------------------------\n");
    return 0;
}
