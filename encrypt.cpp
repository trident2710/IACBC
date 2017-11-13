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
  block ivbl = block(0xffffffffffffffff);
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

    block block1 (0xff);
    std::cout<<block1<<"\n";
    block enc = encrypt_block(block1, K1);
    std::cout<<enc<<"\n";
    block dec = decrypt_block(enc, K1);
    std::cout<<dec<<"\n";




}
