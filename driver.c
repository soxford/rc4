/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: driver.c
 * Author: Simon Campbell, soxford1379@gmail.com
 * Description: Driver file for rc4 output generation
 * License: GPL
 * Date: 31 December 2014
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "rc4.h"

int main(int argc, const char *argv[])
{
    //create a file to output multiple runs of the rc4 cipher to and run the algorithm multiple times to store information
    FILE* outputF = fopen("rc4_output.txt", "a");
    //null check
    if (outputF == NULL) {
        printf("Error: file failed to open\n");
        return 1;
    }
    
    //assign space for the key;
    uint8_t* key = (uint8_t*)  malloc(sizeof(uint8_t)*keyLength);
    
    //null check
    if (key == NULL) {
        printf("Error: malloc failed");
        return 1;
    }
    //set number of runs of the PRG algo
    int loopcount = 10000;
    
    srand(time(NULL));
    //variables for measuring clock usage
    clock_t begin, end;
    double time_spent;
    begin = clock();
    
    //loop to generate multiple stream outputs
    for (int i = 0; i < loopcount; i++) {
        
        //random key generation
        for (int i = 0; i < keyLength; i++) {
            key[i] = (uint8_t) (rand() % 256);
        }
        uint8_t permutationArray[permutationArrayLength];
        //set output length
        int outputLength = 256;
        
        //run RC4 algorithm
        pseudoRandomGeneration(permutationArray, key, keyLength, outputLength, outputF);
        fprintf(outputF, "\n");
    }
    
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    //report time_spent
    printf("Time spent in PRG loop in seconds: %e\n", time_spent);
    //free the key space
    free(key); 
    //close the file and return
    fclose(outputF);
    return 0;}
