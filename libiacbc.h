#ifndef LIBIACBC_H
#define LIBIACBC_H

#include <iostream>
#include <bitset>

#define BLOCK_SIZE 128
#define KEY_SIZE 128
typedef std::bitset<BLOCK_SIZE> block;

/**
  makes XOR for 2 blocks
  @return res
**/
block xor_block(block block1, block block2);

/**
  increments block
  @return block1+1
**/
block incr_block(block block1);


/**
  adds a padding to the block
  see PKCS#7
**/
block pad(block block1);

/**
  removes a padding from the block
  see PKCS#7
**/
block unpad(block block1);


#endif
