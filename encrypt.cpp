#include "libiacbc.h"
#include <iostream>
#include <bitset>

int main(int argc, char const *argv[]) {
  block block1 (0b1100);
  block block2 (0b1111);
  block res;
  std::cout << block1 << '\n';
  std::cout << block2 << '\n';
  xor_block(block1,block2,&res);
  std::cout << res << '\n';
  block blockenc (0b0);
  for(int i=0;i<1<<5;i++){
    blockenc = incr_block(blockenc);
    std::cout << blockenc << '\n';
  }
  return 0;
}
