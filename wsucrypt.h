#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEBUG 1

// A helper method to read the secret key and assign the global variable.
// This should only be called once at the beginning of the program.
void getkey(char* keyfile);

// The key scheduler. It will access the 64-bit key stored in 'key.txt' and
// generate a subkey based on the input x.
unsigned char keyschedule(int x);
