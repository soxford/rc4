/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: noRe-KeyingTest.cpp
 * Author: Simon Campbell, <simonhmcampbell@gmai.com>
 * Description: A test without re-keying the rc4 cipher for each stream output to compare against the control test and identify bottelnecks
 * License: GPL
 * Date: April 2015
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include "../../MT19937_RandomSource.cpp"
#define STREAM_OUTPUT_LENGTH 257

//variables used in documenting the test
const char* TEST_NAME = "No Re-Keying Test";
const char* FIELDS = "Number of RC4 Streams & Time Spent Initializing and Generating RC4 Streams (s)";

//log_2(the maximum number of pages collected in any one timing test round)
int MAX_PAGE_COUNT_POWER =  20;
long histograms[STREAM_OUTPUT_LENGTH][RC4Stream::PERMUTATION_ARRAY_LENGTH]; //TODO consider potential for cache misses with this data structure, perhaps buffer output?

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
   for (int i = 0; i < STREAM_OUTPUT_LENGTH; i++) {
      for (int j = 0; j < RC4Stream::PERMUTATION_ARRAY_LENGTH; j++) {
         histograms[i][j] = 0;
      }
   }

   //assign space for the key;
   RC4Stream::Key key;

   //allocate space for the RC4 stream
   RC4Stream rc4Stream;

   //initialize Random Number Generation algorithm 
   MT19937_RandomSource randomSource;

   randomSource.initializeRandomNoGen();

   //choose a key and schedule the key but never reschedule the key
   //random key generation
   randomSource.selectRandomKey(key);
   
   //rekey
   rc4Stream.keySchedule(key);

   //variables for measuring clock usage
   clock_t begin, end;
   double time_spent;
   int loopcount;
   //record test data
   logFile << "Test Data:" << endl;
   logFile << FIELDS << endl;

   //try various loop counts to compare speed
   for (int pageCountPower = 8; pageCountPower <= MAX_PAGE_COUNT_POWER; pageCountPower+=3) {
       loopcount = (1 << pageCountPower);
      begin = clock();

      //loop to generate multiple stream outputs
      for (int i = 0; i < loopcount; i++) {
        
        //random key generation still included even though the key is not reset to compare precisely the rekeying step to the control test
        randomSource.selectRandomKey(key);
         
        //TEST rekey is not done in this test
        //rc4Stream.keySchedule(key);
         
        //run RC4 stream algorithm and collect output in histogram counters
        for (int i = 0; i < STREAM_OUTPUT_LENGTH; i++) {
           histograms[i][rc4Stream.PRGRound()]++; //increment the relevant histogram count
        }
      }

      end = clock();
      time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
      //report time_spent
      logFile << "2^{" << pageCountPower << '}' << " & " << time_spent << endl;
   }

   //close the file and return
   logFile.close();
   return 0;
}

