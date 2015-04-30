/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: driver.cpp //TODO convert to c++ idioms
 * Author: Simon Campbell, <simonhmcampbell@gmai.com>
 * Description: Test for timing of rc4 output generation of single bytes in the early key stream (upto position 257)
 * License: GPL
 * Date: April 2015
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//TODO edit time checks to system time checks
#include <stdio.h>
#include <iostream>
#include <time.h>
#include "MT19937_RandomSource.cpp"

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
   long histograms[outputLength][RC4Stream::PERMUTATION_ARRAY_LENGTH]; //TODO consider potential for cache misses with this data structure, perhaps buffer output?
   for (int i = 0; i < outputLength; i++) {
      for (int j = 0; j < RC4Stream::PERMUTATION_ARRAY_LENGTH; j++) {
         histograms[i][j] = 0;
      }
   }

   //assign space for the key;
   RC4Stream::Key *key = new RC4Stream::Key();

   //null check
   if (key == NULL) {
     printf("Error: creation of key failed");
     return 1;
   }
   
   //allocate space for the RC4 stream
   RC4Stream *rc4Stream = new RC4Stream();
   if (rc4Stream == NULL) {
      printf("Error: failed to construct RC4Stream\n");
      return 1;
   }

   
   //initialize Random Number Generation algorithm 
   RC4Stream::Key::RandomSource *randomSource = new MT19937_RandomSource();
   if (randomSource == NULL) {
      printf("Error: failed to construct RandomSource\n");
      return 1;
   }
   randomSource->initializeRandomNoGen();

   //variables for measuring clock usage
   clock_t begin, end;
   double time_spent;

   //try various loop counts to compare speed
   for (int loopcount = 1; loopcount <= 1000000; loopcount*=10) {
      begin = clock();

      //loop to generate multiple stream outputs
      for (int i = 0; i < loopcount; i++) {
        
        //random key generation
        randomSource->selectRandomKey(key);
         
        //rekey
        rc4Stream->keySchedule(key);
         
        //run RC4 stream algorithm and collect output in histogram counters
        for (int i = 0; i < outputLength; i++) {
           histograms[i][rc4Stream->PRGRound()]++; //increment the relevant histogram count
        }
      }

      end = clock();
      time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
      //report time_spent
      fprintf(logFile, "Time spent in sampling loop with loopcount %d in seconds: %e\n", loopcount, time_spent);
   }

   //free the key space
   delete randomSource;
   delete rc4Stream;
   delete key;

   //close the file and return
   fclose(logFile);
   return 0;
}

