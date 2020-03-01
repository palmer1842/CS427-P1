// Jake Palmer
// 2/29/20
// main.c

// This program implements the WSU-CRYPT block-encryption algortihm outlined in
// Project 1 for CS 427. Encryption is done in the following steps:
// - Input Whitening
// - Round Function (16 Rounds)
// - Output Whitening

// The program has two modes of operation, encryption and decryption.

// See README.txt for further documentation

#include "wsucrypt.h"

// GLOBAL
// The secret key used in encryption and decryption
unsigned long int key;

// Convert a char to its hex value
// From Stack Overflow. See References in README.
unsigned short chartohex(unsigned char c) {
  if (c >= '0' && c <= '9') {
    c = c - '0';
  } else {
    c = c - 'a' + 10;
  }
  return c;
}

// Read in a block from the input file. If encrypting, read as an ASCII text
// file. Partial blocks are padded with zeros. If decrypting, read as a HEX
// text file.

// Parameters:
// -- fd: the file descriptor to read from
// -- word: the array of four words to store the block in
// -- encrypt: the current mode of the program

// Returns:
// -- 1: Success
// -- 0: End Of File

int getblock(int fd, unsigned short* word, int encrypt) {
  int num;
  if (encrypt) {  // read as ASCII
    for (int i = 0; i < 4; i++) {
      num = read(fd, &word[i], 2);
      if (num == -1) {
        perror("Error reading from input file");
        close(fd);
        exit(1);
      }
      if (DEBUG) printf("Bytes read from plaintext: %d\n", num);

      // if first read returns zero, done reading
      if (num == 0 && i == 0) return 0;

      // pad half read word with a zero
      if (num == 1) {
        word[i] = (word[i] & 0x00ff) ^ 0x3000;
      }

      // if read returns zero, pad remaining words with '0'
      if (num == 0) {
        for (int j = i; j < 4; j++) {
          word[j] = 0x3030;
        }
      }

      // swap the bytes, since read places them in reverse order
      word[i] = (word[i] << 8) | (word[i] >> 8);
    }
  } else {  // read as HEX
    unsigned char c;
    unsigned short temp;
    for (int i = 0; i < 4; i++) {
      word[i] = 0;
      for (int j = 12; j >= 0; j -= 4) {
        num = read(fd, &c, 1);
        if (num == -1) {
          perror("Error reading from input file");
          close(fd);
          exit(1);
        }
        if (num == 0) return 0;
        temp = chartohex(c);
        word[i] = word[i] ^ (temp << j);
      }
    }
  }

  return 1;
}

// Whiten a block by XORing with the key.
// The result is store directly in the block.

// Parameters:
// -- w: the array of words the block is stored in.

// Returns:
// nothing

void whiten(unsigned short* w) {
  for (int i = 0; i < 4; i++) {
    int j = 3 - i;
    w[i] = w[i] ^ (key >> (16 * j));
    if (DEBUG) printf("%04x", w[i]);
  }
  if (DEBUG) printf("\n");
}

// The main program, handling command line input, file processing, and the
// main algorithm loop. It reads in from the input file one block at a time
// and performs the encryption or decryption, depending on the given mode.
// It writes each processed block to the output file before retrieving the
// next one.

int main(int argc, char** argv) {
  // check for proper number of arguments
  if (argc != 4) {
    printf("Usage: wsucrypt (-e | -d) inputfile keyfile\n");
    return 1;
  }

  // check flag for encrypt/decrypt
  int encrypt;
  if (!strcmp(argv[1], "-e")) {
    encrypt = 1;
  } else if (!strcmp(argv[1], "-d")) {
    encrypt = 0;
  } else {
    printf("Usage: wsucrypt (-e | -d) inputfile keyfile\n");
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
    readfd = open(inputfile, O_RDONLY);
    writefile = fopen("ciphertext.txt", "w");
  } else {
    readfd = open(inputfile, O_RDONLY);
    writefile = fopen("plaintext.txt", "w");
  }

  // error checking for input file
  if (readfd == -1) {
    perror("Error opening input file");
    return 1;
  }

  // start encryption loop over blocks in the file
  while (getblock(readfd, word, encrypt)) {
    if (DEBUG) {
      for (int i = 0; i < 4; i++) {
        printf("word[%d]: %04x  ", i, word[i]);
      }
      printf("\n");
    }

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
          printf("%04x", word[i]);
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
        fprintf(writefile, "%04x", word[i]);
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
