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
block encrypt_block(block block1, const unsigned char* key);

/**
  decrypts given 128 bits block with the given 256 bits key
**/
block decrypt_block(block block1, const unsigned char* key);

/**
  writes bits of the block in unsigend char* of length BLOCK_SIZE/8 bytes
**/
void block_to_char_array(block block1, unsigned char* res);

/**
  writes bits to the block from unsigend char* of length BLOCK_SIZE/8 bytes
**/
block char_array_to_block(const unsigned char* res);

/**
  makes AES-256-IACBC encrypt for plain and writes result to encrypted
  length of encrypted should be length of plain+2
**/
void encrypt_iacbc(const unsigned char* K1, const unsigned char* K2, const unsigned char* R, unsigned char** plain, unsigned char** encrypted, int plength);

/**
  makes AES-256-IACBC decrypt for encrypted and writes result to plain
  length of plain should be length of encrypted-2
**/
void decrypt_iacbc(const unsigned char* K1, const unsigned char* K2, const unsigned char* R, unsigned char** plain, unsigned char** encrypted, int elength);

/**
  IACBC encrypt message which consists of blocks of length BLOCK_SIZE using password of length = pwdlen bytes and IV of length = BLOCK_SIZE/8 bytes
  msglen - number of blocks
**/
void encrypt(const char* pwd, int pwdlen, const unsigned char* IV, unsigned char** msg, int msglen, unsigned char** res);

/**
  IACBC decrypt cipher which consists of blocks of length BLOCK_SIZE using password of length = pwdlen bytes and IV of length = BLOCK_SIZE/8 bytes
  cyphlen - number of blocks
**/
void decrypt(const char* pwd, int pwdlen,  const unsigned char* IV, unsigned char** cyph, int cyphlen, unsigned char** res);



#endif
