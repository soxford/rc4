/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: driver.cpp 
 * Author: Simon Campbell, <simonhmcampbell@gmail.com>
 * Description: Test for timing of rc4 output generation of single bytes in the early key stream (upto position 257)
 * License: GPL
 * Date: April 2015
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/***************************
THIS CODE WILL NOT COMPILE, CLASS RC4Stream AND ITS INNER CLASSES HAVE BEEN REFACTORED TO REMOVE ALL HEAP ALLOCATION
***************************/
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include "../../MT19937_RandomSource.cpp"

int outputLength = 257;
int MAX_PAGE_COUNT_POWER = 20;

using namespace std;

int main(int argc, const char *argv[])
{
   char* fileName = "logfile.txt";
   if (argc > 1) {
      fileName = argv[1];
   }

   //open log file for writing
   ofstream logFile ;
   logFile.open(fileName);
   if(!logFile.is_open()){
      cout << "Error: failed to open file named " << fileName << endl;
      return 1;
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
     logFile << "Error: creation of key failed" << endl;
     return 1;
   }
   
   //allocate space for the RC4 stream
   RC4Stream *rc4Stream = new RC4Stream();
   if (rc4Stream == NULL) {
      logFile << "Error: failed to construct RC4Stream" << endl;
      return 1;
   }

   
   //initialize Random Number Generation algorithm 
   RC4Stream::Key::RandomSource *randomSource = new MT19937_RandomSource();
   if (randomSource == NULL) {
      logFile << "Error: failed to construct RandomSource" << endl;
      return 1;
   }
   randomSource->initializeRandomNoGen();

   //variables for measuring clock usage
   clock_t begin, end;
   double time_spent;
    int loopcount;
   //try various loop counts to compare speed
   for (int pageCountPower = 8; pageCountPower <= MAX_PAGE_COUNT_POWER; pageCountPower+=3) {
       loopcount = (1 << pageCountPower);
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
      logFile << "Time spent in sampling loop with loopcount " << "2^{" << pageCountPower << '}' << " in seconds: " << scientific << time_spent << endl;
   }

   //clean up
   delete randomSource;
   delete rc4Stream;
   delete key;

   //close the file and return
   logFile.close();
   return 0;
}

