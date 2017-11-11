#include "libiacbc.h"
#include "stdio.h"


block xor_block(block block1, block block2){
  return block1^block2;
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

/**
  find the last non-zero octet in the block
  return -1 if block is all zero
**/
int find_last_octet(block block1){
  int i=BLOCK_SIZE;
  while(!block1[i]&&(i>0)) i--;
  return i?i/8:block1[0]?0:-1;
}

/**
 fill octet of block with the value 0x1<@val<0x10
**/
block fill_octet(block block1, int octet_num, int val){
  for(int i=0;i<8;i++){
    block1[8*octet_num+i]=(val&(1<<i))?1:0;
  }
  return block1;
}

/**
  get the padding value of PKCS#7 i.e. how many octets were padded
**/
int get_padding_value(block block1){
  int val = 0;
  for(int i=0;i<8;i++){
    if(block1[BLOCK_SIZE-8+i]) val+=(1<<i);
  }
  return val;
}

block pad(block block1){
  int last_octet = find_last_octet(block1);
  int fill_number = BLOCK_SIZE/8-last_octet-1;
  for(int i = last_octet+1;i<BLOCK_SIZE/8;i++){
    block1 = fill_octet(block1,i,fill_number);
  }
  return block1;
}

block unpad(block block1){
  int padding_val = get_padding_value(block1);
  for(int i = BLOCK_SIZE-8*padding_val;i<BLOCK_SIZE;i++){
    block1[i]=0;
  }
  return block1;
}
