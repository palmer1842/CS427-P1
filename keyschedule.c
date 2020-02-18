#include "wsucrypt.h"

// GLOBAL
// The secret key
extern unsigned long int key;

unsigned char keyschedule(int x) {
  // rotate the key left 1-bit
  key = (key << 1) | (key >> 63);  
 
  // return the byte at x mod 8 in the key
  return (key >> (8 * (x % 8))) & 0xff;
}
