/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: driver.c
 * Author: Simon Campbell, <simonhmcampbell@gmai.com>
 * Description: Test for timing of rc4 output generation of single bytes in the early key stream (upto position 257)
 * License: GPL
 * Date: April 2015
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "rc4.h"
#include "basic_rand.h"
int outputLength = 257;

int main(int argc, const char *argv[])
{
   const char* fileName = "logfile.txt";
   if (argc > 1) {
      fileName = argv[1];
   }
   //open log file for writing
   FILE* logFile = fopen(fileName, "a");
   if(logFile == NULL){
      printf("Error: failed to open file named %s for appending", fileName);
   }

   //allocate space to hold table of results
   long histograms[outputLength][PERMUTATION_ARRAY_LENGTH]; //TODO consider potential for cache misses with this data structure, perhaps buffer output?
   for (int i = 0; i < outputLength; i++) {
      for (int j = 0; j < PERMUTATION_ARRAY_LENGTH; j++) {
         histograms[i][j] = 0;
      }
   }

   //assign space for the key;
   uint8_t* key = (uint8_t*)  malloc(sizeof(uint8_t)*KEY_LENGTH);

   //null check
   if (key == NULL) {
     printf("Error: malloc failed");
     return 1;
   }
   
   //allocate space for the RC4 stream
   RC4Stream* rc4Stream = malloc(sizeof(RC4Stream));
   if (rc4Stream == NULL) {
      printf("Error: malloc failed to allocate RC4Stream\n");
      return 1;
   }

   //allocate space for the permutation Array
   rc4Stream->permutationArray = malloc(sizeof(uint8_t)*PERMUTATION_ARRAY_LENGTH);
    if (rc4Stream->permutationArray == NULL) {
       printf("Error: malloc failed to allocate permutation Array\n");
       return 1;
    }
   
   //initialize Random Number Generation algorithm 
   initializeRandomNoGen();

   //variables for measuring clock usage
   clock_t begin, end;
   double time_spent;

   //try various loop counts to compare speed
   for (int loopcount = 1; loopcount <= 1000000; loopcount*=10) {
      begin = clock();

      //loop to generate multiple stream outputs
      for (int i = 0; i < loopcount; i++) {
        
        //random key generation
        for (int i = 0; i < KEY_LENGTH; i++) {
            key[i] = getRand();
        }
        
        //run RC4 algorithm
        for (int i = 0; i < outputLength; i++) {
           histograms[i][rc4PRGRound(rc4Stream)]++; //increment the relevant histogram count
        }
      }

      end = clock();
      time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
      //report time_spent
      fprintf(logFile, "Time spent in sampling loop with loopcount %d in seconds: %e\n", loopcount, time_spent);
   }

   //free the key space
   free(key); 
   free(rc4Stream->permutationArray);
   free(rc4Stream);
   //close the file and return
   fclose(logFile);
   return 0;
}

