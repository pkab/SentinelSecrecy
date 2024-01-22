/**
 * Sentinel Secrecy filehandler.c - Abhinava Phukan
 */

#include <stdio.h>
#include <string.h>
#include "seclib.h"

/**
 * WriteFile() - Write the Data into the file.
 * @param fname - file name to open
 * @param fsize - file size
 */
void WriteFile(const char* fname, const unsigned char* data, size_t fsize){
    FILE *file = fopen(fname, "wb");
    fwrite(data, 1, fsize, file);
    fclose(file);
}
/**
 * ReadFile() - Read the file and return its contents
 * @param fname - file name to open
 * @param fsize - file size
 */
unsigned char* ReadFile(const char* fname, size_t *fsize){
	FILE *file = fopen(fname, "rb");
	if(!file){
		perror("Error Opening File");
		exit(EXIT_FAILURE);
	}

	fseek(file, 0, SEEK_END);
	*fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	unsigned char* data = malloc(*fsize);
	if(!data){
		perror("Error Allocating Memory");
		fclose(file);
		exit(EXIT_FAILURE);
	}

	if(fread(data, 1, *fsize, file) != *fsize){
		perror("Error Reading File");
		fclose(file);
		free(data);
		exit(EXIT_FAILURE);
	}

	fclose(file);
	return data;
}
