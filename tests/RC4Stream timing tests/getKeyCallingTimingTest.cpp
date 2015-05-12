/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: keyLookupRC4StreamTest.cpp
 * Author: Simon Campbell, <simonhmcampbell@gmail.com>
 * Description: A timing test with Key Lookup implementation that tests timing of this
 * License: GPL
 * Date: April 2015
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include "../../MT19937_RandomSource.cpp"

#ifndef RC4_STREAM_GUARD
#include "../../RC4Stream.cpp"
#endif
// an implementation of RC4 Stream::Key to test timing
class KeyLookupTest : public RC4Stream::Key {
   public:
      KeyLookupTest() : RC4Stream::Key() {}
      ~KeyLookupTest() {}

      virtual uint8_t getModuloLength(int i) {
         //TEST static return to test whether the calling process is delayed
         return 0x00; //RC4Stream::Key::_key[i % RC4Stream::Key::KEY_LENGTH];
      }
};
//variables used in documenting the test
const char* TEST_NAME = "Key Get Modulo Length Calling Timing test in RC4 Stream";
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
   //allocate space to hold table of results
   long histograms[STREAM_OUTPUT_LENGTH][RC4Stream::PERMUTATION_ARRAY_LENGTH]; //TODO consider potential for cache misses with this data structure, perhaps buffer output?
   for (int i = 0; i < STREAM_OUTPUT_LENGTH; i++) {
      for (int j = 0; j < RC4Stream::PERMUTATION_ARRAY_LENGTH; j++) {
         histograms[i][j] = 0;
      }
   }

   //assign space for the key;
   KeyLookupTest key;
   
   //allocate space for the RC4 stream
   RC4Stream rc4Stream;

   //initialize Random Number Generation algorithm 
   RC4Stream::Key::RandomSource randomSource;
   randomSource.initializeRandomNoGen();

   //variables for measuring clock usage
   clock_t begin, end;
   double time_spent;
   
   //record test data
   logFile << "Test Data:" << endl;
   logFile << FIELDS << endl;

   //try various loop counts to compare speed
   for (int loopcount = 1; loopcount <= MAX_LOOPCOUNT; loopcount*=10) {
      begin = clock();

      //loop to generate multiple stream outputs
      for (int i = 0; i < loopcount; i++) {
        
        //random key generation
        randomSource.selectRandomKey(key);
         
        //rekey
        rc4Stream.keySchedule(key);
         
        //run RC4 stream algorithm and collect output in histogram counters
        for (int i = 0; i < STREAM_OUTPUT_LENGTH; i++) {
           histograms[i][rc4Stream.PRGRound()]++; //increment the relevant histogram count
        }
      }

      end = clock();
      time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
      //report time_spent
      logFile << loopcount << " & " << time_spent << endl;
   }

   //close the file and return
   logFile.close();
   return 0;
}

