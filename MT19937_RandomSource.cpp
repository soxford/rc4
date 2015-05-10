/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: MT19937_RandomSource.cpp
 * Author: Simon Campbell, <simonhmcampbell@gmail.com>
 * Description: Defines random initialization, getRand and select Random Key functions using the standard mt19937 random number generator
 * License: GPL
 * Date: April 2015  
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef RC4_STREAM_GUARD
#include "RC4Stream.cpp"
#endif

#include <random>

class MT19937_RandomSource : public RC4Stream::Key::RandomSource {
   private:
      //Instantiate a mersenne twister object
      std::mt19937 MT;
   public:
      //constructor
      MT19937_RandomSource() {}
   
      //destructor
      ~MT19937_RandomSource() {}

      //Initializes the random number generator
      void initializeRandomNoGen(){
              MT.seed(time(0));
      }

      //generate the random key - assumes the key provided is of length KEY_LENGTH bytes
      void selectRandomKey(RC4Stream::Key *key){
              //get the number of required randome numbers from the twister
              int noOfRandsRequired = RC4Stream::Key::KEY_LENGTH/sizeof(std::mt19937::result_type) + 1;
              int randoms[noOfRandsRequired];
              for(int i = 0; i < noOfRandsRequired; i++){
                      randoms[i] = MT();	
              }

              for(int i = 0; i < RC4Stream::Key::KEY_LENGTH; i++){
                      //TODO review this assignment to confirm correctness
                      key->setModuloLength(i, (uint8_t) ((randoms[i*sizeof(uint8_t)/sizeof(std::mt19937::result_type)] >> (sizeof(uint8_t)*(i % sizeof(std::mt19937::result_type)) )) & 0xff));
              }
      }

};




