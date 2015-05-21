/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: rc4example.cpp
 * Author: Simon Campbell, <simonhmcampbell@gmai.com>
 * Description: Example to provide assemly for inspection
 * License: GPL
 * Date: May 2015
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "RC4Stream.cpp"

using namespace std;

int main(int argc, const char *argv[])
{
   //output length of rc4 stream  
   int outputLength = 16;
 
   //assign space for the key;
   RC4Stream::Key key;

   uint8_t keys_bytes[RC4Stream::Key::KEY_LENGTH] = 
                          { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

   //allocate space for the RC4 stream
   RC4Stream rc4Stream;

   
      for(int j = 0; j < RC4Stream::Key::KEY_LENGTH; j++) {
         key.setModuloLength(j, keys_bytes[j]);
      }
     //schedule key
     rc4Stream.keySchedule(key);
      
     //run RC4 stream algorithm and collect output in histogram counters
     for (int j = 0; j < outputLength; j++) {
      	rc4Stream.PRGRound();
     }

   //close the file and return
   return 0;
}

