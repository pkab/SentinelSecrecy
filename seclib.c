/**
 * Sentinel Secrecy seclib.c - Abhinava Phukan
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "seclib.h"


/**
 * KeyGen - Generates the key and writes it in a key file
 * keyFile - file location of the key.
 */
void KeyGen(const char* keyFile){
	FILE *keyf = fopen(keyFile, "wb");
	unsigned char key[KEY_SIZE / 8];
    	RAND_bytes(key, sizeof(key));
	fwrite(key, 1, sizeof(key), keyf);
	fclose(keyf);
}
/**
 * SecEnc() - Do the secure AES Encryption
 * @param fileName - File to read in order to encrypt
 * @param keyFile - Key To Generate and use for encryption
 */
void SecEnc(const char* fileName, const char* keyFile){
    if(access(keyFile, F_OK) != -1){
        /* Do Nothing */
    }else {
        /* Generate the Key File if not available found */
        KeyGen(keyFile);
	}

	AES_KEY aeskey; /* AES Key to be used */
	unsigned char key[KEY_SIZE / 8];
	/* Read the Key File Generated */
	FILE* keyf = fopen(keyFile, "rb");
	if(!keyf){
		perror("Error Opening File");
		exit(EXIT_FAILURE);
	}
    /* Read the key binary data */
	if(fread(key, 1, sizeof(key), keyf) != sizeof(key)){
		perror("Error reading key");
		fclose(keyf);
		exit(EXIT_FAILURE);
	}
	fclose(keyf);

	/* Initialize EVP API for encryption */
	EVP_CIPHER_CTX *ctx;
	ctx = EVP_CIPHER_CTX_new();
	if(!ctx) {
        perror("CTX init error.");
        exit(EXIT_FAILURE);
	}
	/* Set the Symmetric key encryption */
	if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, NULL) != 1) {
        perror("Error"); exit(EXIT_FAILURE);
    }

	/* Find out the size of the plain text */
	size_t fsize;
	unsigned char* pt = ReadFile(fileName, &fsize);

    /* Padding is a technique to ensure plaintext to be encrypted fits neatly in a fixed size blocks */
	int padsize = ((fsize / BLOCK_SIZE) + 1) * BLOCK_SIZE;
	unsigned char* ct = malloc(padsize);
	if(!ct){
		perror("Error allocating Memory");
		free(pt);
		exit(EXIT_FAILURE);
	}
    /* Block wise padding for AES encryption and EVP api to encrypt the data*/
    EVP_CIPHER_CTX_set_padding(ctx, padsize);
    int outlen =0 ;
    if(EVP_EncryptUpdate(ctx, ct, (int*)&padsize, pt, fsize) != 1) {
        perror("Error in encryption");
        exit(1);
    }
    if (EVP_EncryptFinal_ex(ctx, ct + padsize, (int *)&outlen) != 1) {
        perror("Error in encryption");
        exit(1);
    }
    padsize+=outlen;

	/* Generate the output Ciphertext File */
	char* outFile = (char*)malloc(strlen(fileName) + 13);
	sprintf(outFile, "%s%s", fileName,".secret.txt");
	WriteFile(outFile, ct, padsize);
	printf("Encryption Successful. Ciphertext Stored in %s.\n", outFile);

	/* Clear Data After use */
	free(outFile);
	free(pt);
	free(ct);
	EVP_CIPHER_CTX_free(ctx);
}
/**
 * SecDec() - Do the secure AES Decryption
 * @param fileName - File to read in order to decrypt
 * @param keyFile - Key To Generate and use for decryption
 */
void SecDec(const char* fileName, const char* keyFile){
	FILE* keyf = fopen(keyFile, "rb");
	if(!keyf){
		perror("Error Opening File");
		exit(EXIT_FAILURE);
	}
	AES_KEY aeskey;
	unsigned char key[KEY_SIZE / 8];
	if(fread(key, 1, sizeof(key), keyf) != sizeof(key)){
		perror("Error reading key");
		fclose(keyf);
		exit(EXIT_FAILURE);
	}
	fclose(keyf);

    /* Initialize EVP API for decryption */
    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();

    if (!ctx){
        perror("Error");
        exit(1);
    }
    /* Initialize Symmetric key for Decryption */
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, NULL) != 1){
        perror("Error");
        exit(1);
    }
	size_t fsize;

	unsigned char* ct = ReadFile(fileName, &fsize);
	size_t padsize = ((fsize / BLOCK_SIZE) + 1) * BLOCK_SIZE;
    EVP_CIPHER_CTX_set_padding(ctx, padsize);
	unsigned char* pt = malloc(fsize);
	if(!pt){
		perror("Error Allocating memory");
		free(ct);
		exit(EXIT_FAILURE);
	}

    /* Evp API for decryption */
    if (EVP_DecryptUpdate(ctx, pt, (int *)&padsize, ct, fsize) != 1) {
        perror("Update Error");
        exit(1);
    }
    int outlen=0;
    if (EVP_DecryptFinal_ex(ctx, pt + padsize, (int *)&outlen) != 1) {

        perror("Final Error");
        exit(1);
    }
    padsize += outlen;
    pt[padsize-1]='\0';
	printf("Decryption Successful: %s\n", pt);

    /* Clear Data After use */
	free(ct);
	free(pt);
	EVP_CIPHER_CTX_free(ctx);
}

