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
   
   //assign space for the key, the RC4 stream and output
   RC4Stream::Key key;
   RC4Stream rc4Stream;
   uint8_t output[257];


   uint8_t key_bytes[RC4Stream::Key::KEY_LENGTH] = 
                          { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

   
   for(int j = 0; j < RC4Stream::Key::KEY_LENGTH; j++) {
      key.setModuloLength(j, key_bytes[j]);
   }

   //schedule key
   rc4Stream.keySchedule(key);
      
   //run RC4 stream algorithm to get the first 257 bytes of output
   rc4Stream.PRGOutputFirst257BytesToArray(output);

   //return
   return 0;
}

