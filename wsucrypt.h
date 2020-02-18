#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// The key scheduler. It will access the 64-bit key stored in 'key.txt' and
// generate a subkey based on the input x.
unsigned char keyschedule(int x);
