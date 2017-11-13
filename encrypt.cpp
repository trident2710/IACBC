#include "libiacbc.h"
#include <iostream>
#include <bitset>

int main(int argc, char const *argv[]) {
    char* password = "password";
    int plen = 8;
    unsigned char* iv = (unsigned char*)malloc(BLOCK_SIZE/8);
    block ivbl = char_array_to_block((unsigned char*)"testtesttesttest");
    block_to_char_array(ivbl, iv);

    unsigned char** v = (unsigned char**)malloc(2*sizeof(unsigned char*));
    for(int i = 0; i < 2; i++){
        block b = char_array_to_block((unsigned char*)(!i?"helohelohelohela\n":"helohelohelohelb\n"));
        v[i] = (unsigned char*)malloc(BLOCK_SIZE/8);
        block_to_char_array(b, v[i]);
    }

    unsigned char** e = (unsigned char**)malloc(4*sizeof(unsigned char*));
    for(int i = 0; i < 4; i++){
        e[i] = (unsigned char*)malloc(BLOCK_SIZE/8);
    }
    encrypt(password, 8, iv, v, 2, e);

    unsigned char** dv = (unsigned char**)malloc(2*sizeof(unsigned char*));
    for(int i = 0; i < 2; i++){
        dv[i] = (unsigned char*)malloc(BLOCK_SIZE/8);
    }
    decrypt(password, 8, iv, e, 4, dv);

    printf("p------\n");
    for(int i=0;i<2;i++){
      byte_dump(v[i],BLOCK_SIZE/8);
      printf("\n");
    }

    printf("e------\n");
    for(int i=0;i<4;i++){
      byte_dump(e[i], BLOCK_SIZE/8);
      printf("\n");
    }

    printf("d------\n");
    for(int i=0;i<2;i++){
      byte_dump(dv[i],BLOCK_SIZE/8);
      printf("\n");
    }

    return 0;
}
