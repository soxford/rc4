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
      
      /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
       * RC4 Pseudo Random Generation Algorithm batch output method - outputs the first 16 bytes
       * Description: Generates the next out_length bytes of output from the RC4 stream
       * input - a useable output array to store the output bytes.
       * precondition - 16 <= length of the array, 
       * percondition - WARNING to use this method correctly it must be the case that no PRG rounds can have been called since the last key scheduling.
       * output - void
       * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
      virtual void PRGOutputFirst16BytesToArray(uint8_t output_array[]) {
	    uint8_t tmpI, tmpJ, total, result;
	    // it is assumed that _i = _j = 0, as now PRG yet done
	    // INITIAL BLOCK
	    tmpI = _permutationArray[1]; //lookup, _i = 0
	    _j = (_j + tmpI) & MASK_8;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[1] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 2
	    tmpI = _permutationArray[2];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[0] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[2] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 3
	    tmpI = _permutationArray[3];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[1] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[3] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 4
	    tmpI = _permutationArray[4];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[2] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[4] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 5
	    tmpI = _permutationArray[5];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[3] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[5] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 6
	    tmpI = _permutationArray[6];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[4] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[6] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 7
	    tmpI = _permutationArray[7];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[5] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[7] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 8
	    tmpI = _permutationArray[8];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[6] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[8] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 9
	    tmpI = _permutationArray[9];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[7] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[9] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 10
	    tmpI = _permutationArray[10];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[8] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[10] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 11
	    tmpI = _permutationArray[11];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[9] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[11] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 12
	    tmpI = _permutationArray[12];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[10] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[12] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 13
	    tmpI = _permutationArray[13];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[11] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[13] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 14
	    tmpI = _permutationArray[14];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[12] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[14] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 15
	    tmpI = _permutationArray[15];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[13] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[15] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 16
	    tmpI = _permutationArray[16];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[14] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[16] = tmpJ;		  
	    total = tmpI + tmpJ;

	    _i = 16;	//ensure _i is correct

	    result = _permutationArray[total];  
	    output_array[15] = result; //final allocation
      }

      /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
       * RC4 Pseudo Random Generation Algorithm batch output method - outputs the first 257 Bytes
       * Description: Generates the next out_length bytes of output from the RC4 stream
       * input - a useable output array to store the output bytes.
       * precondition - 257 <= length of the array, 
       * percondition - WARNING to use this method correctly it must be the case that no PRG rounds can have been called since the last key scheduling.
       * output - void
       * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
      virtual void PRGOutputFirst257BytesToArray(uint8_t output_array[]) {
      }
};


