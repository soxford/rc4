/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title:  
 * Author: Simon Campbell, <simonhmcampbell@gmail.com>
 * Description: A test of the doublebyte collection code using a 4x256x256 histogram to check cache improvements
 * License: 
 * Date: August 2015
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include <mpi.h>
#include "../../../../MT19937_RandomSource.cpp"
//TEST use this to mask for mod 4
#define MASK_2 0x03
#define HISTOGRAM_I_COUNT 4
//variables used in documenting the test
const char* TEST_NAME = "Double Byte Test with histogram i index size reduced to 4";
const char* FIELDS = "Number of RC4 Bytes & Time Spent Initializing and Generating and Recording histograms from the RC4 Bytes";
//log_2(the maximum number of bytes collected in any one timing test round)
int MAX_BYTE_COUNT_POWER =  28;
//Moved to static to avoid stack overflow
//allocate space to hold table of results TEST 4x256x256
long histograms[HISTOGRAM_I_COUNT][RC4Stream::PERMUTATION_ARRAY_LENGTH][RC4Stream::PERMUTATION_ARRAY_LENGTH]; 
using namespace std;

int main(int argc, char *argv[])
{
  int my_proc_id;

  /*Initialize MPI*/
  MPI_Init(&argc, &argv);
  
  /*Work out how this process fits with the others*/
  MPI_Comm_rank(MPI_COMM_WORLD,&my_proc_id);
  //Name the log file accordingly
  string fileName = "ArcusTestData" + to_string((long long) my_proc_id) + ".txt";


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

   //clear the histogram
   for (int i = 0; i < HISTOGRAM_I_COUNT; i++) {
      for (int j = 0; j < RC4Stream::PERMUTATION_ARRAY_LENGTH; j++) {
          for (int k = 0; k < RC4Stream::PERMUTATION_ARRAY_LENGTH; k++) {
            histograms[i][j][k] = 0;
          }
      }
   }

   //assign space for the key;
   RC4Stream::Key key;

   //allocate space for the RC4 stream
   RC4Stream rc4Stream;

   //initialize Random Number Generation algorithm 
   MT19937_RandomSource randomSource;
   //TODO consider seeding procedure
   randomSource.initializeRandomNoGen();

   //variables for measuring clock usage
   clock_t begin, end;
   double time_spent;
   uint8_t first_byte, second_byte; //for collection the byte value pairs
   int bytecount = 0; //for count the number of bytes output in a stream
   //record test data
   logFile << "Test Data:" << endl;
   logFile << FIELDS << endl;

   //try various stream lengths for comparison, bytecount_power is log_2(the number of bytes to be output in the stream
   for (int bytecount_power = 16 ; bytecount_power <= MAX_BYTE_COUNT_POWER; bytecount_power += 3 ) {
      bytecount = (1 << bytecount_power);
      //begin timing
      begin = clock();
      //random key generation
      randomSource.selectRandomKey(key);
     
      //rekey
      rc4Stream.keySchedule(key);
      
      //skip the first (1 << 10) bytes of output
      for (int skipPosition = 0; skipPosition < (1 << 10) ; skipPosition++) {
        rc4Stream.PRGRound();
      }
      //loop to generate the output stream and collect results
      first_byte = rc4Stream.PRGRound();
      
      for (int i = 0; i < bytecount; i++) {
         second_byte = rc4Stream.PRGRound();
         //TEST use MASK_2 instead of MASK_8 as the i index has size 4
         //TEST histograms[i & MASK_8][first_byte][second_byte]++; //increment the relevant histogram count
         histograms[i & MASK_2][first_byte][second_byte]++; //increment the relevant histogram count
         first_byte = second_byte;
      }

      end = clock();
      time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
      //report time_spent
      logFile << "2^{" << bytecount_power << "}" << " & " << time_spent << endl;
   }

   //close the file and return
   logFile.close();

   MPI_Finalize();
   exit(EXIT_SUCCESS);
}

