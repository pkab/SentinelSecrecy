#ifndef SECLIB_H_INCLUDED
#define SECLIB_H_INCLUDED

/**
 * Sentinel Secrecy seclib.h - Abhinava Phukan
 */

#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

#define KEY_SIZE 256
#define BLOCK_SIZE 16


/**
 * using in the unsigned char* format as when dealing with raw binary data
 * Refer : https://stackoverflow.com/questions/13642381/c-c-why-to-use-unsigned-char-for-binary-data
 * 1. To avoid sign issues when dereferencing ( +/- issues when dereferencing the value ) Range only between 0-255
 * 2. This allows consistent byte representation.
 * 3. Hold clarity that we are dealing with raw binary data. Unknown intent could lead way for bitwise operations
 * that may give room for buffer overflow.
 */


void KeyGen(const char* keyFile);
void SecEnc(const char* fileName, const char* keyFile);
void SecDec(const char* fileName, const char* keyFile);
void WriteFile(const char* fname, const unsigned char* data, size_t fsize);
unsigned char* ReadFile(const char* fname, size_t *fsize);


#endif // SECLIB_H_INCLUDED
