/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: controlTest.cpp 
 * Author: Simon Campbell, <simonhmcampbell@gmail.com>
 * Description: A threaded test where, timing is now done using the wall clock instead of the cpu usage rate.
 * License: GPL
 * Date: June 2015
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include "../../MT19937_RandomSource.cpp"
#include<thread>

//variables used in documenting the test
const char* TEST_NAME = "Naive Threading Test, no efforst to provide different seeding for each thread";
const char* FIELDS = "Number of RC4 Streams & Time Spent Initializing and Generating RC4 Streams (s)";
int STREAM_OUTPUT_LENGTH = 257;

int MAX_LOOPCOUNT = 1000000;
int NUM_THREADS = 4;
using namespace std;

void call_from_thread(int tid, string fileNamePrefix) {
    string fileName = fileNamePrefix + to_string(tid);
   //open log file for appending data
   ofstream logFile ;
   logFile.open(fileName, ofstream::app);
   if(!logFile.is_open()){
      cout << "Error: failed to open file named " << fileName << endl;
      return;
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
   RC4Stream rc4Stream;

   //initialize Random Number Generation algorithm 
   MT19937_RandomSource randomSource;

   randomSource.initializeRandomNoGen();

   //variables for measuring clock usage
   struct timespec begin, end;
   double time_spent;
   
   //record test data
   logFile << "Test Data:" << endl;
   logFile << FIELDS << endl;

   //try various loop counts to compare speed
   for (int loopcount = 1; loopcount <= MAX_LOOPCOUNT; loopcount*=10) {
      clock_gettime(CLOCK_MONOTONIC, &begin);

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

      clock_gettime(CLOCK_MONOTONIC, &end);
      time_spent = (end.tv_sec - begin.tv_sec);
      time_spent += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
      //report time_spent
      logFile << loopcount << " & " << time_spent << endl;
   }

   //close the file and return
   logFile.close();
   return;
}

//MAIN
int main(int argc, const char *argv[])
{
       thread t[NUM_THREADS];
   string fileName;
   if (argc > 1) {
      fileName = string(argv[1]);
   } else {
      cout << "Usage: Enter file name for storing test data" << endl;
      return 1;
   }
          
   for (int i = 0; i < NUM_THREADS; i++) { 
      t[i] = thread(call_from_thread, i, fileName);
   }

   for (int i = 0; i < NUM_THREADS; i++){
       t[i].join();
   }
   return 0;
}
