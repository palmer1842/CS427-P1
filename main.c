// Jake Palmer
// 2/22/20
// main.c

// This program implements the WSU-CRYPT block-encryption algortihm outlined in
// Project 1 for CS 427. Encryption is done in the following steps:
// - Input Whitening
// - Round Function (16 Rounds)
// - Output Whitening

#include "wsucrypt.h"

// GLOBAL
// The secret key used in encryption and decryption
unsigned long int key;

int main(int argc, char** argv) {
  // read in the key from 'key.txt'
  FILE* file = fopen("key.txt", "r");
  fscanf(file, "%lx", &key);
  fclose(file);

  printf("Key: %lx\n", key);
  return 0;
}
