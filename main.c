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

// Read in the plaintext
void getwords(int fd, unsigned short* word) {
  for (int i = 0; i < 4; i++) {
    read(fd, &word[i], 2);
    // swap the bytes, since read places them in reverse order
    word[i] = (word[i] << 8) | (word[i] >> 8);
  }
}

int main(int argc, char** argv) {
  unsigned short word[4];
  unsigned short temp[4];

  // read in the key from 'key.txt'
  getkey("key.txt");
  if (DEBUG) printf("Key: %lx\n", key);

  // open plaintext file for reading
  int plainfd = open("plaintext.txt", O_RDONLY);
  getwords(plainfd, word);
  if (DEBUG) {
    for (int i = 0; i < 4; i++) {
      printf("word[%d]: %hu  ", i, word[i]);
    }
    printf("\n");
  }

  // TEMPORARY WORDS FOR TESTING
  word[0] = 0x0123;
  word[1] = 0x4567;
  word[2] = 0x89ab;
  word[3] = 0xcdef;

  // input whitening
  if (DEBUG) printf("Input Whitening: ");
  for (int i = 0; i < 4; i++) {
    int j = 3 - i;
    word[i] = word[i] ^ (key >> (16 * j));
    if (DEBUG) printf("%x", word[i]); 
  }
  if (DEBUG) printf("\n");

  for (int round = 0; round < 16; round++) {
    // F function
    unsigned short f[2];
    f_function(word[0], word[1], round, f);
    
    // get temp[0]
    temp[0] = word[2] ^ f[0];
    temp[0] = (temp[0] >> 1) | (temp[0] << 15);

    // get temp[1]
    temp[1] = (word[3] << 1) | (word[3] >> 15);
    temp[1] = word[3] ^ f[1];

    // get temp[2]
    temp[2] = word[0];

    // get temp[3]
    temp[3] = word[1];

    // assign temps as new words for next round
    word[0] = temp[0];
    word[1] = temp[1];
    word[2] = temp[2];
    word[3] = temp[3];
  }

  // output whitening

  close(plainfd);
}
