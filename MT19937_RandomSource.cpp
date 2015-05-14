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
#define MASK_8 0xff
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
      void selectRandomKey(RC4Stream::Key &key){
	      //get the number of required randome numbers from the twister
              /* general version
	      int noOfRandsRequired = RC4Stream::Key::KEY_LENGTH/sizeof(std::mt19937::result_type) + 1;
	      int randoms[noOfRandsRequired];
              */
	      //inline version
	      
	      int r0 = MT();
	      int r1 = MT();
	      int r2 = MT();
	      int r3 = MT();

	      
    	      /* general loop version
              for(int i = 0; i < RC4Stream::Key::KEY_LENGTH; i++){
                      key.setModuloLength(i, (uint8_t) ((randoms[i*sizeof(uint8_t)/sizeof(std::mt19937::result_type)] >> (sizeof(uint8_t)*8*(i % sizeof(std::mt19937::result_type)) )) & MASK_8));
              }
	      */
		//unravelled version
		
		key.setModuloLength(0, (uint8_t) (r0 & MASK_8));
		key.setModuloLength(1, (uint8_t) ((r0 >> 8) & MASK_8));
		key.setModuloLength(2, (uint8_t) ((r0 >> 16) & MASK_8));
		key.setModuloLength(3, (uint8_t) ((r0 >> 24) & MASK_8));

		key.setModuloLength(4, (uint8_t) (r1 & MASK_8));
		key.setModuloLength(5, (uint8_t) ((r1 >> 8) & MASK_8));
		key.setModuloLength(6, (uint8_t) ((r1 >> 16) & MASK_8));
		key.setModuloLength(7, (uint8_t) ((r1 >> 24) & MASK_8));

		key.setModuloLength(8, (uint8_t) (r2 & MASK_8));
		key.setModuloLength(9, (uint8_t) ((r2 >> 8) & MASK_8));
		key.setModuloLength(10, (uint8_t) ((r2 >> 16) & MASK_8));
		key.setModuloLength(11, (uint8_t) ((r2 >> 24) & MASK_8));

		key.setModuloLength(12, (uint8_t) (r3 & MASK_8));
		key.setModuloLength(13, (uint8_t) ((r3 >> 8) & MASK_8));
		key.setModuloLength(14, (uint8_t) ((r3 >> 16) & MASK_8));
		key.setModuloLength(15, (uint8_t) ((r3 >> 24) & MASK_8));
      }

};




