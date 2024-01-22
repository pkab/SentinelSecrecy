/**
 * Sentinel Secrecy main.c - Abhinava Phukan
 */
#include <stdio.h>
#include <string.h>
#include "seclib.h"

int main(int argc, char** argv){
    printf("sentsec - AES message encryption/decryption software 0.1 - Developed by Abhinava Phukan\n\n");
    if(argc < 2){
        printf("Usage: %s [options] [[FILE KEY] | command]\n\n", "sentsec");
        printf("Try help for more help.\n\n");
        return 1;
    }
    else if(argc == 2){
        const char* option = argv[1];
        if(strcmp(option, "help") == 0){
            printf("Usage: %s [options] FILE KEY\n\n", "sentsec");
            printf("Options:\n\n");
            printf("%4senc: Encrypt the File. Generates a ciphertext secret file and a key. If key exist, it can be used otherwise new key is generated.\n","");
            printf("%4sdec: Decrypt the Secret File. Generates a plaintext output based on the key provided for decryption.\n\n","");
            printf("Try %s help <enc/dec> for more detailed help.\n\n", "sentsec");
        }
        else {
            printf("Error: try \"%s help\" for help\n\n", "sentsec");
        }
    }
    else if(argc == 3){
        const char* option = argv[1];
        const char* ref = argv[2];
        if(strcmp(option, "help") == 0){
            if(strcmp(ref, "enc") == 0){
                printf("Usage: %s enc <file_to_encrypt> <key_file_<input/generate>>\n\n","sentsec");
            }
            else if(strcmp(ref, "dec") == 0){
                printf("Usage: %s dec <file_to_decrypt> <key_file_input>\n\n","sentsec");
            }
            else {
                printf("Error: try \"%s help\" for help\n\n", "sentsec");
            }
        }
        else {
            printf("Error: try \"%s help\" for help\n\n", "sentsec");
        }
    }else if(argc == 4){
        const char* option = argv[1];
        if(strcmp(option, "enc") == 0) SecEnc(argv[2], argv[3]);
        else if(strcmp(option, "dec") == 0) SecDec(argv[2], argv[3]);
    }else {
        printf("Error: Too many arguments (expected 4 max). Try \"%s help\" for help\n\n","sentsec");
    }
    return 0;
}
