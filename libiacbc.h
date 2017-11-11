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
void xor_block(block block1, block block2, block *res);

/**
  increments block
  @return block1+1
**/
block incr_block(block block1);


#endif
