/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: rc4Test.cpp
 * Author: Simon Campbell, <simonhmcampbell@gmai.com>
 * Description: Test for correctness of output of RC4Stream
 * License: GPL
 * Date: April 2015
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include <cstdio>
#include <iostream>
#include <fstream>
#include <ctime>
#include "../../MT19937_RandomSource.cpp"

using namespace std;

int main(int argc, const char *argv[])
{
   
   //assign space for the key;
   RC4Stream::Key key;

   //allocate space for the RC4 stream
   RC4Stream rc4Stream;

   //initialize Random Number Generation algorithm 
   MT19937_RandomSource randomSource;

   randomSource.initializeRandomNoGen();

   
   //variables for measuring clock usage
   clock_t begin, end;
   double time_spent;

   begin = clock();
   for (int i = 0; i < 3; i++) {
      //assign the key value
      randomSource.selectRandomKey(key);
      //show key
      for(int j = 0; j < RC4Stream::Key::KEY_LENGTH; j++) {
         cout << (int) key._key[j] << ' ';
      }
      cout << endl;

     //schedule key
     rc4Stream.keySchedule(key);
      
   }
   end = clock();
   time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
   cout << "Time spent sampling in seconds: " << scientific << time_spent << endl;

   //close the file and return
   return 0;
}

