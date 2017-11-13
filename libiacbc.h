#ifndef LIBIACBC_H
#define LIBIACBC_H

#include <iostream>
#include <bitset>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/conf.h>
#include <openssl/err.h>

#define BLOCK_SIZE 128 //length in bits
#define KEY_SIZE 256 //length in bits
#define IACBC_KEY_SIZE 80 //length in bytes
#define ITER 10000
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

/**
  generates the key using openssl PKCS5_PBKDF2_HMAC_SHA1 function
  @return 1 on success 0 on error
**/
int gen_key(const char* password, int passlen, const unsigned char* IV, int IVlen, unsigned char *out);

/**
  displays unsigned char* byte by byte in binary form (big endian)
**/
void byte_dump(const unsigned char* block, int block_length);

/**
  encrypts given 128 bits block with the given 256 bits key
**/
block encrypt_block(block block1, unsigned char* key);

/**
  decrypts given 128 bits block with the given 256 bits key
**/
block decrypt_block(block block1, unsigned char* key);

/**
  writes bits of the block in unsigend char* of length BLOCK_SIZE/8 bytes
**/
void block_to_char_array(block block1, unsigned char* res);

/**
  writes bits to the block from unsigend char* of length BLOCK_SIZE/8 bytes
**/
block char_array_to_block(const unsigned char* res);

#endif
