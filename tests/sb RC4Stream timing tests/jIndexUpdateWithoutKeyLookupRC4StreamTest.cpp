/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: jIndexUpdateWithoutKeyLookupRC4StreamTest.cpp
 * Author: Simon Campbell, <simonhmcampbell@gmail.com>
 * Description: A timing test with Key Schedule implementation that updates the j index but does not get anything from the key
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
// an implementation of RC4 Stream that has no key rescheduling behaviour
class JIndexUpdateWithoutKeyLookupRC4Stream : public RC4Stream {
   public:
      JIndexUpdateWithoutKeyLookupRC4Stream() : RC4Stream(){}

      ~JIndexUpdateWithoutKeyLookupRC4Stream() {}

      virtual void keySchedule(RC4Stream::Key &key) {
         //initialize the permutation array to be the identity permutation
         for (int i = 0; i < RC4Stream::PERMUTATION_ARRAY_LENGTH ; i++) {
            RC4Stream::_permutationArray[i] = i;
         }
         //schedule the permutation array
         unsigned int j = 0;
         for (int i = 0; i < RC4Stream::PERMUTATION_ARRAY_LENGTH; i++) {
            j = (j + (unsigned int) RC4Stream::_permutationArray[i] /*TEST + (unsigned int) key.getModuloLength(i)*/) % RC4Stream::PERMUTATION_ARRAY_LENGTH;
            //swap ith and jth elements
            uint8_t tmp = RC4Stream::_permutationArray[i];
            RC4Stream::_permutationArray[i] = RC4Stream::_permutationArray[j];
            RC4Stream::_permutationArray[j] = tmp;  
         }
         //initialize the state variables 
         RC4Stream::_i = 0;
         RC4Stream::_j = 0;
      }

};

//variables used in documenting the test
const char* TEST_NAME = "Permutation Scheduling with no key lookup, RC4 Stream Test";
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
   RC4Stream::Key key;
   
   //allocate space for the RC4 stream
   JIndexUpdateWithoutKeyLookupRC4Stream rc4Stream;

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

