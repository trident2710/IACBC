#include "libiacbc.h"
#include "stdio.h"


void xor_block(block block1, block block2, block *res){
  *res = block1^block2;
}

block incr_block(block block1){
  bool carry = true;
  int i=0;
  while (carry){
    bool v = block1[i];
    carry = !(v^1);
    block1[i]=carry?0:1;
    i++;
  };
  return block1;
}
