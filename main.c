// Jake Palmer
// 2/29/20
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
int getblock(int fd, unsigned short* word) {
  int num;
  for (int i = 0; i < 4; i++) {
    num = read(fd, &word[i], 2);
    if (num == 0) {
      // pad the unused words with zero
      for (int j = i; j < 4; j++) {
        word[j] = 0;
      }
      return i;
    }
    // swap the bytes, since read places them in reverse order
    word[i] = (word[i] << 8) | (word[i] >> 8);
  }

  return num;
}

// Whiten a block by XORing with the key
void whiten(unsigned short* w) {
  for (int i = 0; i < 4; i++) {
    int j = 3 - i;
    w[i] = w[i] ^ (key >> (16 * j));
    if (DEBUG) printf("%x", w[i]); 
  }
  if (DEBUG) printf("\n");
}

int main(int argc, char** argv) {
  // check for proper number of arguments
  if (argc != 4) {
    printf("Usage: wsucrypt (-e | -d) inputfile.txt key.txt\n");
    return 1;
  }

  // check flag for encrypt/decrypt
  int encrypt;
  if (!strcmp(argv[1], "-e")) {
    encrypt = 1;
  } else if (!strcmp(argv[1], "-d")) {
    encrypt = 0;
  } else {
    printf("Usage: wsucrypt (-e | -d) inputfile.txt key.txt\n");
    return 1;
  }

  // assign input files
  char* inputfile = argv[2];
  char* keyfile = argv[3];

  // storage for each block as it is processed
  unsigned short word[4];
  unsigned short temp[4];

  // read in the key from the provided key file
  getkey(keyfile);
  if (DEBUG) printf("Key: %lx\n", key);

  // open input and output files for reading and writing
  int readfd;
  FILE* writefile;
  if (encrypt) {
    readfd = open(inputfile, O_RDONLY | O_CREAT,
                                   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    writefile = fopen("ciphertext.txt", "w");
  } else {
    readfd = open(inputfile, O_RDONLY | O_CREAT,
                                   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    writefile = fopen("plaintext.txt", "w");
  }

  // start encryption loop over blocks in the file
  while (getblock(readfd, word)) {
    if (DEBUG) {
      for (int i = 0; i < 4; i++) {
        printf("word[%d]: %hu  ", i, word[i]);
      }
      printf("\n");
    }

    // TEMPORARY WORDS FOR TESTING
    /*if (encrypt) {
      word[0] = 0x0123;
      word[1] = 0x4567;
      word[2] = 0x89ab;
      word[3] = 0xcdef;
    } else {
      word[0] = 0x9a76;
      word[1] = 0xd6d5;
      word[2] = 0x78c4;
      word[3] = 0x4766;
    }*/

    // input whitening
    if (DEBUG) printf("Input Whitening: ");
    whiten(word);

    // round function loop
    for (int round = 0; round < 16; round++) {
      if (DEBUG) printf("ROUND %d\n", round);

      // F function
      unsigned short f[2];
      f_function(word[0], word[1], round, f, encrypt);

      // get temp[0]
      if (encrypt) {
        temp[0] = word[2] ^ f[0];
        temp[0] = (temp[0] >> 1) | (temp[0] << 15);
      } else {
        temp[0] = (word[2] << 1) | (word[2] >> 15);
        temp[0] = temp[0] ^ f[0];
      }

      // get temp[1]
      if (encrypt) {
        temp[1] = (word[3] << 1) | (word[3] >> 15);
        temp[1] = temp[1] ^ f[1];
      } else {
        temp[1] = word[3] ^ f[1];
        temp[1] = (temp[1] >> 1) | (temp[1] << 15);
      }

      // get temp[2]
      temp[2] = word[0];

      // get temp[3]
      temp[3] = word[1];

      // assign temps as new words for next round
      word[0] = temp[0];
      word[1] = temp[1];
      word[2] = temp[2];
      word[3] = temp[3];

      if (DEBUG) {
        printf("Block: ");
        for (int i = 0; i < 4; i++) {
          printf("%x", word[i]);
        }
        printf("\n");
      }
    }

    // undo last swap from round function
    word[0] = temp[2];
    word[1] = temp[3];
    word[2] = temp[0];
    word[3] = temp[1];

    // output whitening
    if (DEBUG) printf("Output Whitening: ");
    whiten(word);

    // write block to output file
    if (encrypt) {
      // write as HEX
      for (int i = 0; i < 4; i++) {
        fprintf(writefile, "%x", word[i]);
      }
    } else {
      // write as ASCII plaintext
      for (int i = 0; i < 4; i++) {
        char c1 = word[i] >> 8;
        char c2 = word[i];
        fprintf(writefile, "%c%c", c1, c2);
      }
    }
  }

  close(readfd);
  fclose(writefile);

  return 0;
}
