#include "libiacbc.h"
#include <iostream>
#include <bitset>





int main(int argc, char const *argv[]) {


  // block block1 (0b110000000000000001);
  // block block2 (0b1111);
  // block res;
  // std::cout << block1 << '\n';
  // std::cout << block2 << '\n';
  // res = xor_block(block1,block2);
  // std::cout << res << '\n';
  // block blockenc (0b0);
  // for(int i=0;i<1<<5;i++){
  //   blockenc = incr_block(blockenc);
  //   std::cout << blockenc << '\n';
  // }

  // block padtest(1<<8);
  // std::cout << padtest << '\n';
  // padtest = pad(padtest);
  // std::cout << padtest << '\n';
  // padtest = unpad(padtest);
  // std::cout << padtest << '\n';
  // return 0;

  char* password = "password";
  int plen = 8;
  unsigned char* iv = (unsigned char*)malloc(BLOCK_SIZE/8);
  block ivbl = char_array_to_block((unsigned char*)"testtesttesttest");
  block_to_char_array(ivbl, iv);

  unsigned char* res = (unsigned char*)malloc(IACBC_KEY_SIZE);
  unsigned char* K1 = (unsigned char*)malloc(KEY_SIZE/8);
  unsigned char* K2 = (unsigned char*)malloc(KEY_SIZE/8);
  unsigned char* R = (unsigned char*)malloc(KEY_SIZE/8/2);

  gen_key(password, 8, (const unsigned char*)iv, BLOCK_SIZE/8, res);

  memcpy(K1, res, 32);
  memcpy(K2, res+32, 32);
  memcpy(R, res+64, 16);
  //
  // byte_dump(res, 80);
  // std::cout <<'\n';
  // byte_dump(R, 16);
  // std::cout <<'\n';
  // byte_dump(K1, 32);
  // std::cout <<'\n';
  // byte_dump(K2, 32);
  //
  // // std::cout<<res<<"\n\n";
  // // std::cout<<K1<<"\n\n";
  // // std::cout<<K2<<"\n\n";
  // // std::cout<<R<<"\n\n";

  // block block1 (0b1111111111);
  // unsigned char* val = (unsigned char*)malloc(BLOCK_SIZE/8);
  // block_to_char_array(block1, val);
  // std::cout<<block1<<"\n\n";
  // byte_dump(val, BLOCK_SIZE/8);
  // block block2 = char_array_to_block(val);
  // std::cout<<block2<<"\n\n";

    // block block1 (0xff);
    // std::cout<<block1<<"\n";
    // block enc = encrypt_block(block1, K1);
    // std::cout<<enc<<"\n";
    // block dec = decrypt_block(enc, K1);
    // std::cout<<dec<<"\n";


    printf("p------\n");
    unsigned char** v = (unsigned char**)malloc(2*sizeof(unsigned char*));
    for(int i = 0; i < 2; i++){
        block b = char_array_to_block((unsigned char*)(!i?"helohelohelohela":"helohelohelohelb"));
        v[i] = (unsigned char*)malloc(BLOCK_SIZE/8);
        block_to_char_array(b, v[i]);
        byte_dump(v[i],BLOCK_SIZE/8);
        printf("\n");
    }
    unsigned char** e = (unsigned char**)malloc(4*sizeof(unsigned char*));
    for(int i = 0; i < 4; i++){
        e[i] = (unsigned char*)malloc(BLOCK_SIZE/8);
    }
    encrypt_iacbc(K1, K2, R, v, e, 2);
    // for(int i=0;i<4;i++){
    //   byte_dump(e[i], BLOCK_SIZE/8);
    //   printf("\n");
    // }

    unsigned char** dv = (unsigned char**)malloc(2*sizeof(unsigned char*));
    for(int i = 0; i < 2; i++){
        dv[i] = (unsigned char*)malloc(BLOCK_SIZE/8);
    }
    decrypt_iacbc(K1, K2, R, dv, e, 4);
    printf("d------\n");
    for(int i=0;i<2;i++){
      byte_dump(dv[i],BLOCK_SIZE/8);
      printf("\n");
    }

    return 0;


}
