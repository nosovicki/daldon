#include <stdio.h>
#include <stdlib.h>

#include "ac.h"

/* Use random() and srandom() if you have them (Unix-like systems do). */
#define RANDOM random
#define SRANDOM srandom
/* Otherwise, use rand() and srand() from the standard C library. */
/* rand() is not very random, but will do for the demonstration here. */
/* #define RANDOM rand */
/* #define SRANDOM srand */

/* Do not change these to rand and srand, they are declared in stdlib.h. */
/* If the system declares these in a header, these two lines can be removed. */
long random ();
void srandom (unsigned);

#define NUMLOOPS 10000
#define ADAPT 1
#define FILE1 "foo"
#define FILE2 "bar"
#define MASK1 ((0x1<<3) - 1)
#define MASK2 ((0x1<<5) - 1)
#define NSYM1 (MASK1 + 1)
#define NSYM2 (MASK2 + 1)

int
main ()
{
  ac_encoder ace1;
  ac_encoder ace2;
  ac_decoder acd1;
  ac_decoder acd2;
  ac_model acm1;
  ac_model acm2;
  int sym, i;

  FILE *fp = fopen("../ok", "r");
  char c;
   int *counts = malloc(256 * sizeof(int));
  while (EOF != (c = getc(fp))) {
      ++counts[(unsigned char)c];
  }
  ac_encoder_init (&ace1, FILE1);
  //ac_encoder_init (&ace2, FILE2);
  ac_model_init (&acm1, 256, NULL, ADAPT);
  //ac_model_init (&acm2, NSYM2, NULL, ADAPT);

  rewind(fp);
  int len = 0;
  while (EOF != (c = getc(fp))) {
      ac_encode_symbol(&ace1, &acm1, (unsigned char) c);
      ++len;
  }
  /*
  SRANDOM (0);
  for (i=0; i<NUMLOOPS; i++)  {
    sym = (RANDOM() & MASK1);
    ac_encode_symbol (&ace1, &acm1, sym);
    sym = (RANDOM() & MASK2);
    ac_encode_symbol (&ace1, &acm2, sym);
    sym = (RANDOM() & MASK1);
    ac_encode_symbol (&ace2, &acm1, sym);
    sym = (RANDOM() & MASK2);
    ac_encode_symbol (&ace2, &acm2, sym);
  }
  */

  ac_encoder_done (&ace1);
  //ac_encoder_done (&ace2);
  ac_model_done (&acm1);
  //ac_model_done (&acm2);

  printf ("bits - bytes for encoder 1: %d - %d\n", (int) ac_encoder_bits (&ace1), (int) ac_encoder_bits (&ace1) / 8);
  //printf ("bits for encoder 2: %d\n", (int) ac_encoder_bits (&ace2));

  ac_decoder_init (&acd1, FILE1);
  //ac_decoder_init (&acd2, FILE2);
  ac_model_init (&acm1, 256, NULL, ADAPT);
  //ac_model_init (&acm2, NSYM2, NULL, ADAPT);
  rewind(fp);
  while (EOF != (c = getc(fp))) {
      sym = ac_decode_symbol (&acd1, &acm1);
      if (sym != (unsigned char) c) {
          printf ("decoding error: %d != %d\n", c, sym);
          exit (1);
      }
  }
  /*
  SRANDOM (0);
  for (i=0; i<NUMLOOPS; i++)  {
    sym = ac_decode_symbol (&acd1, &acm1);
    if (sym != (RANDOM() & MASK1))  {
      printf ("decoding error\n");
      exit (1);
    }
    sym = ac_decode_symbol (&acd1, &acm2);
    if (sym != (RANDOM() & MASK2))  {
      printf ("decoding error\n");
      exit (1);
    }
    sym = ac_decode_symbol (&acd2, &acm1);
    if (sym != (RANDOM() & MASK1))  {
      printf ("decoding error\n");
      exit (1);
    }
    sym = ac_decode_symbol (&acd2, &acm2);
    if (sym != (RANDOM() & MASK2))  {
      printf ("decoding error\n");
      exit (1);
    }
  }
  */

  ac_decoder_done (&acd1);
  //ac_decoder_done (&acd2);
  ac_model_done (&acm1);
  //ac_model_done (&acm2);

  return 0;
}
