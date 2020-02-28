#include "wsucrypt.h"

unsigned short* f_function(unsigned short w1, unsigned short w2, int round, unsigned short* f, int encrypt) {
  // generate the 12 subkeys
  unsigned char sk[12];
  for (int i = 0; i < 12; i++) {
    if (encrypt) {
      sk[i] = keyschedule(4 * round + (i % 4), encrypt);
    } else {
      sk[i] = keyschedule(4 * (15 - round) + (3 - (i % 4)), encrypt);
    }
  }

  // reverse the list if decrypting
  if (!encrypt) {
    unsigned char temp;
    int j = 11;
    for (int i = 0; i < 6; i++) {
      temp = sk[i];
      sk[i] = sk[j];
      sk[j] = temp;
      j--;
    }
  }

  if (DEBUG) {
    for (int i = 0; i < 12; i++) {
      printf("key %d: %x\n", i, sk[i]);
    }
  }

  unsigned short t0 = gperm(w1, round, sk[0], sk[1], sk[2], sk[3]);
  unsigned short t1 = gperm(w2, round, sk[4], sk[5], sk[6], sk[7]);

  unsigned short concat = sk[8];
  concat = concat << 8 | sk[9];

  f[0] = t0 + (2 * t1) + (concat % 65536);

  concat = sk[10];
  concat = concat << 8 | sk[11];

  f[1] = (2 * t0) + t1 + (concat % 65536);

  if (DEBUG) printf("f0: %x  f1: %x\n", f[0], f[1]);

  return f;
}
