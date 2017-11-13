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

int gen_key(const char* password, int passlen, const unsigned char* IV, int IVlen, unsigned char *out){
    return PKCS5_PBKDF2_HMAC_SHA1(password, passlen, IV, IVlen, ITER, IACBC_KEY_SIZE, out);
}

void block_to_char_array(block block1, unsigned char* res){
  for(int i=0;i<BLOCK_SIZE/8;i++){
    unsigned char val=0;
    for(int j=0;j<8;j++){
      val+=block1[8*i+j]?1<<j:0;
    }
    *(res+i)=val;
  }
}

block char_array_to_block(const unsigned char* res){
  block block1;
  for(int i=0;i<BLOCK_SIZE;i++){
    for(int j=0;j<8;j++){
      block1[8*i+j] = (*(res+i)&(1<<j))?1:0;
    }
  }
  return block1;
}

void byte_dump(const unsigned char* block, int block_length){
  for(int i=0;i<block_length;i++){
    unsigned char x = *(block+i);
    for (int j = 0; j < 8; j++) {
      printf("%d", !!((x << j) & 0x80));
    }
    printf("\n");
  }
}


block encrypt_block(block block1, unsigned char* key){
  unsigned char* bl = (unsigned char*)malloc(BLOCK_SIZE/8);
  block_to_char_array(block1, bl);
  unsigned char* output = (unsigned char*)malloc(BLOCK_SIZE/8);
  AES_KEY aes_key;
  AES_set_encrypt_key(key, KEY_SIZE, &aes_key);
  AES_encrypt(bl, output, &aes_key);
  return char_array_to_block(output);
}

block decrypt_block(block block1, unsigned char* key){
  unsigned char* bl = (unsigned char*)malloc(BLOCK_SIZE/8);
  block_to_char_array(block1, bl);
  unsigned char* output = (unsigned char*)malloc(BLOCK_SIZE/8);
  AES_KEY aes_key;
  AES_set_decrypt_key(key, KEY_SIZE, &aes_key);
  AES_decrypt(bl, output, &aes_key);
  return char_array_to_block(output);
}
