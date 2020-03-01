CS427 Project 1 -- 2/29/20
WSU-CRYPT --- A Block Cipher Encyption program
Jake Palmer --- jake.palmer@wsu.edu

Description:
  This project implements the WSU-CRYPT algorithm as specified in CS 427
  project 1. It is a block cipher operating in ECB mode. Encryption is
  performed by whitening the input, running a round function 16 times,
  and then whitening the output. The round function calls an f function
  with the first half of the block, which performs manipulations using
  12 generated subkeys. Afterwards, the block is permutated before the
  next round.

  Decryption uses the same functions, except that the subkeys are used in
  the reverse order, and some swaps are reversed.

  Input to the program is an ASCII text file for encryption, which is output as
  as HEX text file. I/O is reversed for decryption.

Project Files:
  main.c -- Contains input processing and the main algorithm
  ffunc.c -- Contains the f function
  gperm.c -- Contains the g permutation function with substitution table
  keyschedule.c -- Retrieves the key from keyfile and generates subkeys
  wsucrypt.h --- Header file with function definitions and DEBUG options
  Makefile -- Compilation instructions for 'make'
  README.txt -- Documentation

Compiling:
  A Makefile is included with the project files, so to build the program simply
  run 'make'. No special flags are used during the compilation.

Usage and testing:
  ./wsucrypt (-e | -d) inputfile keyfile

  When run in encryption mode, the contents of inputfile.txt will be read in
  as an ASCII text file, and keyfile.txt should contain a 64 bit HEX key. The
  output will be a HEX ciphertext file called "ciphertext.txt".

  When run in decryption mode, the contents of inputfile.txt will be read in
  as a HEX file, and keyfile.txt should contain the same 64 bit HEX key used
  for encryption. The output will be an ASCII textfile called "plaintext.txt".

  If the output files do not exist, they will be created.

  Set DEBUG in wsucrypt.h to 1 and recompile for detailed output during the
  programs execution.
