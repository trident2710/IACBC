#include "libiacbc.h"
#include "readInput.h"
#include "string.h"
#include <iostream>
#include <bitset>

bool run(iacbc_input_t *params){
  size_t ivsize = BLOCK_SIZE/8;
  unsigned char* iv = (unsigned char*)calloc(BLOCK_SIZE/8,sizeof(char));
  if(!hex2bin(params->IV, ivsize*2, &iv, &ivsize)){
    printf("Impossible to transform IV.\n");
    return false;
  }

  uint8_t *buf=0;
  size_t len=0;
  read_file(params->input, &buf, &len);
  if(params->enc){
    printf("%s\n","encryption mode");
    int block_bytes = BLOCK_SIZE/8;
    int block_count = len/block_bytes;
    int padded_block_count = 0;
    if(len%block_bytes){
      block_count+=1;
      padded_block_count=2*block_count-1;
    } else{
      padded_block_count=2*block_count;
    }

    unsigned char** plain_blocks = (unsigned char**)calloc(padded_block_count,sizeof(unsigned char*));
    for(int i = 0; i < padded_block_count; i++){
        plain_blocks[i] = (unsigned char*)calloc(block_bytes,sizeof(char));
    }

    for(int i=0;i<padded_block_count;i++){
        unsigned char* pblock = (unsigned char*)calloc(block_bytes,sizeof(char));
        int data_size = (i==(padded_block_count-1)&&(len%block_bytes))?len%block_bytes:block_bytes;
        memcpy(pblock,(buf+block_bytes*i/2),data_size);
        block b = i%2?block(0):char_array_to_block(pblock);
        if(i%2||data_size!=block_bytes) b = pad(b);
        block_to_char_array(b,pblock);
        memcpy(plain_blocks[i],pblock,block_bytes);

    }
    int enc_blocks_count = padded_block_count+2;
    unsigned char** encrypted_blocks = (unsigned char**)calloc((enc_blocks_count),sizeof(unsigned char*));
    for(int i = 0; i < enc_blocks_count; i++){
        encrypted_blocks[i] = (unsigned char*)calloc(block_bytes,sizeof(char));
    }
    encrypt(params->pwd, strlen(params->pwd), iv, plain_blocks, padded_block_count, encrypted_blocks);
    unsigned char* enc_data = (unsigned char*)calloc(block_bytes*enc_blocks_count,sizeof(char));
    for(int i=0;i<enc_blocks_count;i++){
      memcpy(enc_data+block_bytes*i,encrypted_blocks[i],block_bytes);
    }
    write_file(params->out,(uint8_t*)enc_data,enc_blocks_count*block_bytes);

    // printf("len: %d\n",len);
    // printf("block count: %d\n",block_count);
    // printf("padded block count: %d\n",padded_block_count);
    // printf("enc blocks count: %d\n",enc_blocks_count);
    printf("%s\n","success!");
    return true;

  } else{
    printf("%s\n","decryption mode");
    int block_bytes = BLOCK_SIZE/8;
    int enc_blocks_count = len/block_bytes;
    int padded_block_count = enc_blocks_count-2;
    int block_count;
    if (padded_block_count %2){
      block_count = (padded_block_count-1)/2+1;
    } else block_count = padded_block_count/2;

    unsigned char** encrypted_blocks = (unsigned char**)calloc((enc_blocks_count),sizeof(unsigned char*));
    for(int i = 0; i < enc_blocks_count; i++){
        encrypted_blocks[i] = (unsigned char*)calloc(block_bytes,sizeof(char));
        memcpy(encrypted_blocks[i],buf+i*block_bytes,block_bytes);
        //byte_dump(encrypted_blocks[i],16);
    }

    unsigned char* plain_text = (unsigned char*)calloc(block_count*block_bytes,sizeof(char));
    unsigned char** plain_blocks = (unsigned char**)calloc(padded_block_count,sizeof(unsigned char*));
    for(int i = 0; i < padded_block_count; i++){
        plain_blocks[i] = (unsigned char*)calloc(block_bytes,sizeof(char));
    }

    decrypt(params->pwd, strlen(params->pwd), iv, encrypted_blocks, enc_blocks_count, plain_blocks);

    for(int i = 0;i<padded_block_count;i++){
      unsigned char* pblock = (unsigned char*)calloc(block_bytes,sizeof(char));
      memcpy(pblock,plain_blocks[i],block_bytes);
      block b = char_array_to_block(pblock);
      if((i==padded_block_count-1)||i%2) b = unpad(b);
      block_to_char_array(b,pblock);
      memcpy(plain_blocks[i],pblock,block_bytes);
    }

    int k=0;
    for(int i=0;i<padded_block_count;i++){
      unsigned char* pblock = (unsigned char*)calloc(block_bytes,sizeof(char));
      memcpy(pblock,plain_blocks[i],block_bytes);
      block b = char_array_to_block(pblock);
      if(b!=block(0)){
        memcpy(plain_text+block_bytes*k,pblock,block_bytes);
        k++;
      }
    }
    write_file(params->out,(uint8_t*)plain_text,block_count*block_bytes);

    // printf("len: %d\n",len);
    // printf("enc blocks count: %d\n",enc_blocks_count);
    // printf("padded block count: %d\n",padded_block_count);
    // printf("block count: %d\n",block_count);
    printf("%s\n","success!");
    return true;

  }

}

int main(int argc, char *const *argv) {
    iacbc_input_t params;
    if(!parse_input(argc, argv, &params)){
      printf("Something went wrong while reading input params.\n");
      return 0;
    }
    if(!run(&params)){
      printf("Operation did not succeed.\n");
      return 0;
    }
    return 1;
}
