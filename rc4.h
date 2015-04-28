/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: rc4
 * Author: Simon Campbell, soxford1379@gmail.com
 * Description: code for the rc4 stream cipher
 * License: GPL
 * Date: 20th December 2014
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <libc.h>

int PERMUTATION_ARRAY_LENGTH = 256; //permutation array length (no of bytes)
int KEY_LENGTH = 10; //key length (no of bytes)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Key Scheduling function
 * input - key, keylength (typically 5 - 16 bytes) and the permutation array (256 bytes long)
 * output - void
 * description - permutes the elements of the permutation array according to the RC4 Key Scheduling Algorithm
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void keySchedule( uint8_t* permutationArray, uint8_t* key, int KEY_LENGTH){
   //initialize the permutation array to be the identity permutation
   for (int i = 0; i < PERMUTATION_ARRAY_LENGTH ; i++) {
      permutationArray[i] = i;
   }
   
   //schedule the permutation array
   unsigned int j = 0;
   for (int i = 0; i < PERMUTATION_ARRAY_LENGTH; i++) {
      j = (j + (unsigned int) permutationArray[i] + (unsigned int) key[i % KEY_LENGTH]) % PERMUTATION_ARRAY_LENGTH;
      //swap ith and jth elements
      uint8_t tmp = permutationArray[i];
      permutationArray[i] = permutationArray[j];
      permutationArray[j] = tmp;  
   }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Pseudo-random generation function 
 * input - the permutation array (256 bytes long), outputSize (>= 1), key and keylength
 * output - void (output bytes to a file as a space separate array of output bytes of length outputSize)
 * description - outputs bytes according to the RC4 Pseudo-random generation Algorithm
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void pseudoRandomGeneration(uint8_t* permutationArray, uint8_t* key, int KEY_LENGTH, int outputSize, FILE* outputFile){
   //if output file is null then return and print error message
   if(outputFile == NULL){
      printf("Error: pseudoRandomNumber generation aborted as output file in NULL\n");
      return;
   } 

   //keySchedule to initialize ahead of Pseudo Random generation
   keySchedule(permutationArray, key, KEY_LENGTH);

   //PRG loop
   int i = 0, j = 0, k = 0;
   while(k < outputSize){
      i = (i + 1) % PERMUTATION_ARRAY_LENGTH;
      j = (j + permutationArray[i]) % PERMUTATION_ARRAY_LENGTH;

      //swap ith and jth elements
      uint8_t tmp = permutationArray[i];
      permutationArray[i] = permutationArray[j];
      permutationArray[j] = tmp;  
      
      //assign the output
      fprintf(outputFile, "%02x ", permutationArray[(permutationArray[i] + permutationArray[j]) % PERMUTATION_ARRAY_LENGTH]); 
      k++;
   }
}
