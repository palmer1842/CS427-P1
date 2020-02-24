#include "wsucrypt.h"

unsigned short* f_function(unsigned short w1, unsigned short w2, int round, unsigned short* f, int encrypt) {
  unsigned short t0 = gperm(w1, round, encrypt);
  unsigned short t1 = gperm(w2, round, encrypt);

  unsigned short concat = keyschedule(4 * round, encrypt);
  concat = concat << 8 | keyschedule(4 * round + 1, encrypt);

  f[0] = t0 + (2 * t1) + (concat % 65536);

  concat = keyschedule(4 * round + 2, encrypt);
  concat = concat << 8 | keyschedule(4 * round + 3, encrypt);

  f[1] = (2 * t0) + t1 + (concat % 65536);

  if (DEBUG) printf("f0: %x  f1: %x\n", f[0], f[1]);

  return f;
}
