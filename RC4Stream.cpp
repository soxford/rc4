/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: RC4Stream.cpp
 * Author: Simon Campbell, <simonhmcampbell@gmail.com>
 * Description: code for the rc4 stream cipher
 * License: GPL
 * Date: April 2015
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <cstdint>

#define RC4_STREAM_GUARD 0
#define MASK_4 0xf
#define MASK_8 0xff
class RC4Stream
{
   public:
      static const int PERMUTATION_ARRAY_LENGTH = 256; //permutation array length (no of bytes), 256 is used in TLS

   protected:
      uint8_t _permutationArray[PERMUTATION_ARRAY_LENGTH]; //the permutation array which must have length PERMUTATION_ARRAY_LENGTH and contain one of each byte value < this length
      int _i; // the first internal state which increments on each output, 0 <= i < PERMUTATION_ARRAY_LENGTH
      int _j; // the second internal state which defined by RC4

   public:

      //constructor
      RC4Stream() : _i(0), _j(0) {
      }

      //destructor
      ~RC4Stream() {
      }
     
      //Key class which instantiates the rc4 key 
      class Key {
         public:
            static const int KEY_LENGTH = 16; //_key length (no of bytes), 16 is used in TLS
	    //BREAK ENCAPSULATION TO AVOID COST OF GETTER METHOD CALL (STACK FRAME SET UO OVERHEAD ETC.)
            uint8_t _key[RC4Stream::PERMUTATION_ARRAY_LENGTH]; // invariant _key[i] == _key[i % KEY_LENGTH]
            Key() {}

            ~Key() {}
            
            //method returns the appropriate byte of the _key, modulo the KEY_LENGTH - assumes that 0 <= i < 256 = PERMUTATION_ARRAY_LENGTH
            virtual uint8_t getModuloLength(int i) {
               return _key[i];
            }
            
            //Method to set the ith (modulo KEY_LENGTH) byte, loop unravelled to optimise
            virtual void setModuloLength( int i, uint8_t byte ) {
               	i = i & MASK_4; //0 <= i < 16
		int j = i + 16;
		int k = i + 32;
               	_key[i] = byte;
		_key[j] = byte;
		i += 48;	// 48 <= i < 62
		_key[k] = byte;
		j += 48;
		_key[i] = byte;
		k += 48;
		_key[j] = byte;
		i += 48;	// 96 <= i < 112
		_key[k] = byte;
		j += 48;
		_key[i] = byte;
		k += 48;
		_key[j] = byte;
		i += 48;	// 144 <= i < 160
		_key[k] = byte;
		j += 48;
		_key[i] = byte;
		k += 48;
		_key[j] = byte;
		i += 48;	// 192 <= i < 208
		_key[k] = byte;
		j += 48;
		_key[i] = byte;
		k += 48;
		_key[j] = byte;
		i += 48;	// 240 <= i < 256
		_key[k] = byte;
		_key[i] = byte;
		
	    }
       

            /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
             * Title: RandomSource
             * Author: Simon Campbell, <simonhmcampbell@gmail.com>
             * Description: Defines an interface for a random number generator for us in providing random RC4Stream::Key values
             * License: GPL
             * Date: April 2015  
             * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
            class RandomSource {
               public:
                  //method to initialize the Random number source
                  virtual void initializeRandomNoGen() {}

                  //method to provide random data to be the value of the provided key
                  virtual void selectRandomKey(RC4Stream::Key &key) {}
                  virtual ~RandomSource() {}
            };

      }; 


      /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
       * Key Scheduling method
       * input - RC4Stream to be initialised, key to be used in initialisation
       * output - void
       * description - permutes the elements of the permutation array according to the RC4 Key Scheduling Algorithm and initialises the internal state variables
       * Assumes that the rc4 stream permutation array and the key array have been assigned!
       * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
       virtual void keySchedule(RC4Stream::Key &key){

         //initialize the permutation array to be the identity permutation
         for (int i = 0; i < PERMUTATION_ARRAY_LENGTH ; i++) {
            _permutationArray[i] = i;
         }
         
         //schedule the permutation array
         unsigned int j = 0;
         for (int i = 0; i < PERMUTATION_ARRAY_LENGTH; i++) {
            j = (j + (unsigned int) _permutationArray[i] + (unsigned int) key._key[i]) & MASK_8;
            //swap ith and jth elements
            uint8_t tmp = _permutationArray[i];
            _permutationArray[i] = _permutationArray[j];
            _permutationArray[j] = tmp;  
         }
         //initialize the state variables 
         _i = 0;
         _j = 0;
      }

      /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
       * RC4 Pseudo Random Generation Algorithm round method
       * Description: Generates the next byte of output from the RC4 stream
       * input - an RC4Stream that has been allocated a permutation array
       * output - the next keystream byte as a uint8_t 
       * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
      virtual uint8_t PRGRound()
      {
            _i = (_i + 1) & MASK_8;
            _j = (_j + _permutationArray[_i]) & MASK_8;

            //swap ith and jth elements
            uint8_t tmp = _permutationArray[_i];
            _permutationArray[_i] = _permutationArray[_j];
            _permutationArray[_j] = tmp;  
            
            //return the output
            return _permutationArray[(_permutationArray[_i] + _permutationArray[_j]) & MASK_8];  
      }
};


