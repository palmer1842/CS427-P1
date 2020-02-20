#include "wsucrypt.h"

// GLOBAL
// The secret key
extern unsigned long int key;

// read in the key from 'key.txt'
void getkey(char* keyfile) {
  FILE* file = fopen(keyfile, "r");
  fscanf(file, "%lx", &key);
  fclose(file);
}

unsigned char keyschedule(int x) {
  // rotate the key left 1-bit
  key = (key << 1) | (key >> 63);  
 
  // return the byte at x mod 8 in the key
  unsigned char subkey = (key >> (8 * (x % 8))) & 0xff;
  //if (DEBUG) printf("Subkey: %x\n", subkey);

  return subkey;
}
