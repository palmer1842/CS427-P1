// Jake Palmer
// 2/29/20
// keyschedule.c

// This file contains two functions.
// getkey() accepts a file name, which should contain a 64 bit hex key.
// getsubkey() computes and returns a single byte subkey based on a given value
// and the current mode.

// Following the project specifications, the stored key will be modified in
// the process of computing each subkey. To facilitate using this dynamic key,
// it is stored in a program global variable.

#include "wsucrypt.h"

// GLOBAL
// The secret key
unsigned long int key;

// read in the key from 'key.txt'
void getkey(char* keyfile) {
  FILE* file = fopen(keyfile, "r");
  if (file == NULL) {
    perror("Error opening keyfile");
    exit(1);
  }
  fscanf(file, "%lx", &key);
  fclose(file);
}

// compute and return a subkey from parameters
unsigned char getsubkey(int x, int encrypt) {
  unsigned char subkey;
  if (encrypt) {
    // rotate the key left 1-bit
    key = (key << 1) | (key >> 63);  
    // return the byte at x mod 8 in the key
    subkey = (key >> (8 * (x % 8))) & 0xff;
  } else {
    subkey = (key >> (8 * (x % 8))) & 0xff;
    // rotate the key right one bit
    key = (key >> 1) | (key << 63);
  }

  return subkey;
}
