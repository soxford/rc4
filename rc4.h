/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: rc4
 * Author: Simon Campbell, <simonhmcampbell@gmail.com>
 * Description: code for the rc4 stream cipher
 * License: GPL
 * Date: April 2015
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <libc.h>

int PERMUTATION_ARRAY_LENGTH = 256; //permutation array length (no of bytes), 256 is used in TLS
int KEY_LENGTH = 16; //key length (no of bytes), 16 is used in TLS

//A struct to represent the state of an RC4 Stream 
typedef struct {
   uint8_t* permutationArray; //the permutation array which must have length PERMUTATION_ARRAY_LENGTH and contain one of each byte value < this length
   int i; // the first internal state which increments on each output, 0 <= i < PERMUTATION_ARRAY_LENGTH
   int j; // the second internal state which defined by RC4
} RC4Stream;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Key Scheduling function
 * input - RC4Stream to be initialised, key to be used in initialisation
 * output - void
 * description - permutes the elements of the permutation array according to the RC4 Key Scheduling Algorithm and initialises the internal state variables
 * Assumes that the rc4 stream permutation array and the key array have been assigned!
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void keySchedule( RC4Stream* rc4Stream, uint8_t* key){

   //initialize the permutation array to be the identity permutation
   for (int i = 0; i < PERMUTATION_ARRAY_LENGTH ; i++) {
      rc4Stream->permutationArray[i] = i;
   }
   
   //schedule the permutation array
   unsigned int j = 0;
   for (int i = 0; i < PERMUTATION_ARRAY_LENGTH; i++) {
      j = (j + (unsigned int) rc4Stream->permutationArray[i] + (unsigned int) key[i % KEY_LENGTH]) % PERMUTATION_ARRAY_LENGTH;
      //swap ith and jth elements
      uint8_t tmp = rc4Stream->permutationArray[i];
      rc4Stream->permutationArray[i] = rc4Stream->permutationArray[j];
      rc4Stream->permutationArray[j] = tmp;  
   }
   //initialize the state variables 
   rc4Stream->i = 0;
   rc4Stream->j = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * RC4 Pseudo Random Generation Algorithm round function
 * Description: Generates the next byte of output from the given RC4 stream
 * input - an RC4Stream that has been allocated a permutation array
 * output - the next keystream byte as a uint8_t 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint8_t rc4PRGRound(RC4Stream* rc4Stream)
{
      rc4Stream->i = (rc4Stream->i + 1) % PERMUTATION_ARRAY_LENGTH;
      rc4Stream->j = (rc4Stream->j + rc4Stream->permutationArray[rc4Stream->i]) % PERMUTATION_ARRAY_LENGTH;

      //swap ith and jth elements
      uint8_t tmp = rc4Stream->permutationArray[rc4Stream->i];
      rc4Stream->permutationArray[rc4Stream->i] = rc4Stream->permutationArray[rc4Stream->j];
      rc4Stream->permutationArray[rc4Stream->j] = tmp;  
      
      //return the output
      return rc4Stream->permutationArray[(rc4Stream->permutationArray[rc4Stream->i] + rc4Stream->permutationArray[rc4Stream->j]) % PERMUTATION_ARRAY_LENGTH];  
}
