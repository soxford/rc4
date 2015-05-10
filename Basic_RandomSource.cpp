/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: Basic_RandomSource.cpp
 * Author: Simon Campbell, <simonhmcampbell@gmail.com>
 * Description: Defines random initialization and getRand functions using the standard rand function
 * License: GPL
 * Date: April 2015  
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef RC4_STREAM_GUARD
#include "RC4Stream.cpp"
#endif

#include <stdlib.h>

class Basic_RandomSource : public RC4Stream::Key::RandomSource {
   private: 
      //provides the next random byte
      uint8_t getRand(){
         return (uint8_t) (rand() % 256);
      }

   public:
      //Initializes the basic random number generator
      void initializeRandomNoGen(){
         srand(time(NULL));
      }


      //generate the random key - assumes the key provided is of length KEY_LENGTH bytes
      void selectRandomKey(uint8_t* key){
              for(int i = 0; i < RC4Stream::Key::KEY_LENGTH; i++){
                      key[i] = getRand();
              }
      }
};
