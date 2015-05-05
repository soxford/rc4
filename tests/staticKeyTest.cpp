/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: staticKeyTest.cpp
 * Author: Simon Campbell, <simonhmcampbell@gmai.com>
 * Description: A timing test for RC4 Stream generation with a single static key instead of random key generation on each stream
 * License: GPL
 * Date: April 2015
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include "../MT19937_RandomSource.cpp"

//variables used in documenting the test
const char* TEST_NAME = "Static Key Test";
const char* FIELDS = "Number of RC4 Streams & Time Spent Initializing and Generating RC4 Streams (s)";
int STREAM_OUTPUT_LENGTH = 257;

int MAX_LOOPCOUNT = 1000000;

using namespace std;

int main(int argc, const char *argv[])
{
   const char* fileName;
   if (argc > 1) {
      fileName = argv[1];
   } else {
      cout << "Usage: Enter file name for storing test data" << endl;
      return 1;
   }

   //open log file for appending data
   ofstream logFile ;
   logFile.open(fileName, ofstream::app);
   if(!logFile.is_open()){
      cout << "Error: failed to open file named " << fileName << endl;
      return 1;
   }
   
   //Output details of the attempted test to the logfile
   logFile << endl << "===============================================================" << endl << endl;
   logFile << "Test Name: " << TEST_NAME << endl;
   time_t t = time(0);   // get time now
   struct tm * now = localtime( & t );
   logFile << "Date/Time: "   << (now->tm_year + 1900) << '-' 
                              << (now->tm_mon + 1) << '-'
                              <<  now->tm_mday << " "
                              <<  now->tm_hour << ":"
                              <<  now->tm_min
                              << endl;
   logFile << "Length of each RC4 stream in bytes: " << STREAM_OUTPUT_LENGTH << endl;
   logFile << "Test Data:" << endl;
   //allocate space to hold table of results
   long histograms[STREAM_OUTPUT_LENGTH][RC4Stream::PERMUTATION_ARRAY_LENGTH]; //TODO consider potential for cache misses with this data structure, perhaps buffer output?
   for (int i = 0; i < STREAM_OUTPUT_LENGTH; i++) {
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

   //Initialize RNG
   randomSource->initializeRandomNoGen();
   // TEST Initialize key for use in static key test
   randomSource->selectRandomKey(key);

   //variables for measuring clock usage
   clock_t begin, end;
   double time_spent;
   
   logFile << FIELDS << endl;

   //try various loop counts to compare speed
   for (int loopcount = 1; loopcount <= MAX_LOOPCOUNT; loopcount*=10) {
      begin = clock();

      //loop to generate multiple stream outputs
      for (int i = 0; i < loopcount; i++) {
        
        //TEST random key generation no longer in this loop but instead done once ahead of the loop
        //randomSource->selectRandomKey(key);
         
        //rekey
        rc4Stream->keySchedule(key);
         
        //run RC4 stream algorithm and collect output in histogram counters
        for (int i = 0; i < STREAM_OUTPUT_LENGTH; i++) {
           histograms[i][rc4Stream->PRGRound()]++; //increment the relevant histogram count
        }
      }

      end = clock();
      time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
      //report time_spent
      logFile << loopcount << " & " << time_spent << endl;
   }



   //clean up
   delete randomSource;
   delete rc4Stream;
   delete key;

   //close the file and return
   logFile.close();
   return 0;
}

