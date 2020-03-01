#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEBUG 0

// A helper method to read the secret key and assign the global variable.
// This should only be called once at the beginning of the program.
void getkey(char* keyfile);

// The key scheduler. It will access the 64-bit key stored in 'key.txt' and
// generate a subkey based on the input x.
unsigned char getsubkey(int x, int encrypt);

// The F function used in each round of the algorithm
void f_function(unsigned short w1, unsigned short w2, int round,
                unsigned short* f, int encrypt);

// The G-Permutation function used by the F function
unsigned short gperm(unsigned short word, unsigned char sk0, unsigned char sk1,
                     unsigned char sk2, unsigned char sk3);
