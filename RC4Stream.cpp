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
         for (int i = 0; i < PERMUTATION_ARRAY_LENGTH ; i += 64) {
             _permutationArray[i] = i;
             _permutationArray[i + 1] = i + 1;
             _permutationArray[i + 2] = i + 2;
             _permutationArray[i + 3] = i + 3;
             _permutationArray[i + 4] = i + 4;
             _permutationArray[i + 5] = i + 5;
             _permutationArray[i + 6] = i + 6;
             _permutationArray[i + 7] = i + 7;
             _permutationArray[i + 8] = i + 8;
             _permutationArray[i + 9] = i + 9;
             _permutationArray[i + 10] = i + 10;
             _permutationArray[i + 11] = i + 11;
             _permutationArray[i + 12] = i + 12;
             _permutationArray[i + 13] = i + 13;
             _permutationArray[i + 14] = i + 14;
             _permutationArray[i + 15] = i + 15;
		
             _permutationArray[i + 16] = i + 16;
             _permutationArray[i + 17] = i + 17;
             _permutationArray[i + 18] = i + 18;
             _permutationArray[i + 19] = i + 19;
             _permutationArray[i + 20] = i + 20;
             _permutationArray[i + 21] = i + 21;
             _permutationArray[i + 22] = i + 22;
             _permutationArray[i + 23] = i + 23;
             _permutationArray[i + 24] = i + 24;
             _permutationArray[i + 25] = i + 25;
             _permutationArray[i + 26] = i + 26;
             _permutationArray[i + 27] = i + 27;
             _permutationArray[i + 28] = i + 28;
             _permutationArray[i + 29] = i + 29;
             _permutationArray[i + 30] = i + 30;
             _permutationArray[i + 31] = i + 31;

             _permutationArray[i + 32] = i + 32;
             _permutationArray[i + 33] = i + 33;
             _permutationArray[i + 34] = i + 34;
             _permutationArray[i + 35] = i + 35;
             _permutationArray[i + 36] = i + 36;
             _permutationArray[i + 37] = i + 37;
             _permutationArray[i + 38] = i + 38;
             _permutationArray[i + 39] = i + 39;
             _permutationArray[i + 40] = i + 40;
             _permutationArray[i + 41] = i + 41;
             _permutationArray[i + 42] = i + 42;
             _permutationArray[i + 43] = i + 43;
             _permutationArray[i + 44] = i + 44;
             _permutationArray[i + 45] = i + 45;
             _permutationArray[i + 46] = i + 46;
             _permutationArray[i + 47] = i + 47;
		
             _permutationArray[i + 48] = i + 48;
             _permutationArray[i + 49] = i + 49;
             _permutationArray[i + 50] = i + 50;
             _permutationArray[i + 51] = i + 51;
             _permutationArray[i + 52] = i + 52;
             _permutationArray[i + 53] = i + 53;
             _permutationArray[i + 54] = i + 54;
             _permutationArray[i + 55] = i + 55;
             _permutationArray[i + 56] = i + 56;
             _permutationArray[i + 57] = i + 57;
             _permutationArray[i + 58] = i + 58;
             _permutationArray[i + 59] = i + 59;
             _permutationArray[i + 60] = i + 60;
             _permutationArray[i + 61] = i + 61;
             _permutationArray[i + 62] = i + 62;
             _permutationArray[i + 63] = i + 63;
         }

         /*//schedule the permutation array
	 int j = 0;
	 uint8_t tmpI;
         for ( int i = 0; i < PERMUTATION_ARRAY_LENGTH; i ++) {
	    //Round 1
            j = (j + _permutationArray[i] +  key._key[i]) & MASK_8;
            //swap ith and jth elements
            tmpI = _permutationArray[i];
            _permutationArray[i] = _permutationArray[j];
            _permutationArray[j] = tmpI;  
	 }
         
         */
	 //schedule the permutation array - unrolled loop
         int i = 0;
	 int j = 0;
	 uint8_t tmpI;
         for ( ; i < PERMUTATION_ARRAY_LENGTH; i += 8) {
	    //Round 1
            j = (j + _permutationArray[i] +  key._key[i]) & MASK_8;
            //swap ith and jth elements
            tmpI = _permutationArray[i];
            _permutationArray[i] = _permutationArray[j];
            _permutationArray[j] = tmpI;  

	    //Round 2
            j = (j + _permutationArray[i + 1] +  key._key[i + 1]) & MASK_8;
            //swap ith and jth elements
            tmpI = _permutationArray[i + 1];
            _permutationArray[i + 1] = _permutationArray[j];
            _permutationArray[j] = tmpI;  

	    //Round 3
            j = (j + _permutationArray[i + 2] +  key._key[i + 2]) & MASK_8;
            //swap ith and jth elements
            tmpI = _permutationArray[i + 2];
            _permutationArray[i + 2] = _permutationArray[j];
            _permutationArray[j] = tmpI;  

	    //Round 4
            j = (j + _permutationArray[i + 3] +  key._key[i + 3]) & MASK_8;
            //swap ith and jth elements
            tmpI = _permutationArray[i + 3];
            _permutationArray[i + 3] = _permutationArray[j];
            _permutationArray[j] = tmpI;  

	    //Round 5
            j = (j + _permutationArray[i + 4] +  key._key[i + 4]) & MASK_8;
            //swap ith and jth elements
            tmpI = _permutationArray[i + 4];
            _permutationArray[i + 4] = _permutationArray[j];
            _permutationArray[j] = tmpI;  
	    
	    //Round 6
            j = (j + _permutationArray[i + 5] +  key._key[i + 5]) & MASK_8;
            //swap ith and jth elements
            tmpI = _permutationArray[i + 5];
            _permutationArray[i + 5] = _permutationArray[j];
            _permutationArray[j] = tmpI;  

	    //Round 7
            j = (j + _permutationArray[i + 6] +  key._key[i + 6]) & MASK_8;
            //swap ith and jth elements
            tmpI = _permutationArray[i + 6];
            _permutationArray[i + 6] = _permutationArray[j];
            _permutationArray[j] = tmpI;  
	    
	    //Round 8
            j = (j + _permutationArray[i + 7] +  key._key[i + 7]) & MASK_8;
            //swap ith and jth elements
            tmpI = _permutationArray[i + 7];
            _permutationArray[i + 7] = _permutationArray[j];
            _permutationArray[j] = tmpI;  
         }
         //initialize the state variables for use in the PRG generation
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
       * RC4 Pseudo Random Generation Algorithm batch output method - outputs the first 257 Bytes
       * Description: Generates the next out_length bytes of output from the RC4 stream
       * input - a useable output array to store the output bytes.
       * precondition - 257 <= length of the array, 
       * percondition - WARNING to use this method correctly it must be the case that no PRG rounds can have been called since the last key scheduling.
       * output - void
       * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
      virtual void PRGOutputFirst257BytesToArray(uint8_t output_array[]) {
	    uint8_t tmpI, tmpJ, total, result;
	    // it is assumed that _i = _j = 0, as no PRG yet done
	    tmpI = _permutationArray[_i + 1]; //lookup, _i = 0
	    _j = (_j + tmpI) & MASK_8;
	    for( /* _i= 0 */ ; _i < 240 ; _i += 16 ) {
		    // INITIAL BLOCK
		   // done before the loop or in prior iteration: tmpI = _permutationArray[_i + 1]; //lookup, _i = 0
		    //done before the loop of in the prior iteration: _j = (_j + tmpI) & MASK_8;

		    tmpJ = _permutationArray[_j];
		    _permutationArray[_j] = tmpI;
		    _permutationArray[_i + 1] = tmpJ;		  
		    total = tmpI + tmpJ;
		    //Round 2
		    tmpI = _permutationArray[_i + 2];
		    result = _permutationArray[total];  
		    _j = (_j + tmpI) & MASK_8;

		    output_array[_i + 0] = result;

		    tmpJ = _permutationArray[_j];
		    _permutationArray[_j] = tmpI;
		    _permutationArray[_i + 2] = tmpJ;		  
		    total = tmpI + tmpJ;
		    //Round 3
		    tmpI = _permutationArray[_i + 3];
		    result = _permutationArray[total];  
		    _j = (_j + tmpI) & MASK_8;

		    output_array[_i + 1] = result;

		    tmpJ = _permutationArray[_j];
		    _permutationArray[_j] = tmpI;
		    _permutationArray[_i + 3] = tmpJ;		  
		    total = tmpI + tmpJ;
		    //Round 4
		    tmpI = _permutationArray[_i + 4];
		    result = _permutationArray[total];  
		    _j = (_j + tmpI) & MASK_8;

		    output_array[_i + 2] = result;

		    tmpJ = _permutationArray[_j];
		    _permutationArray[_j] = tmpI;
		    _permutationArray[_i + 4] = tmpJ;		  
		    total = tmpI + tmpJ;
		    //Round 5
		    tmpI = _permutationArray[_i + 5];
		    result = _permutationArray[total];  
		    _j = (_j + tmpI) & MASK_8;

		    output_array[_i + 3] = result;

		    tmpJ = _permutationArray[_j];
		    _permutationArray[_j] = tmpI;
		    _permutationArray[_i + 5] = tmpJ;		  
		    total = tmpI + tmpJ;
		    //Round 6
		    tmpI = _permutationArray[_i + 6];
		    result = _permutationArray[total];  
		    _j = (_j + tmpI) & MASK_8;

		    output_array[_i + 4] = result;

		    tmpJ = _permutationArray[_j];
		    _permutationArray[_j] = tmpI;
		    _permutationArray[_i + 6] = tmpJ;		  
		    total = tmpI + tmpJ;
		    //Round 7
		    tmpI = _permutationArray[_i + 7];
		    result = _permutationArray[total];  
		    _j = (_j + tmpI) & MASK_8;

		    output_array[_i + 5] = result;

		    tmpJ = _permutationArray[_j];
		    _permutationArray[_j] = tmpI;
		    _permutationArray[_i + 7] = tmpJ;		  
		    total = tmpI + tmpJ;
		    //Round 8
		    tmpI = _permutationArray[_i + 8];
		    result = _permutationArray[total];  
		    _j = (_j + tmpI) & MASK_8;

		    output_array[_i + 6] = result;

		    tmpJ = _permutationArray[_j];
		    _permutationArray[_j] = tmpI;
		    _permutationArray[_i + 8] = tmpJ;		  
		    total = tmpI + tmpJ;
		    //Round 9
		    tmpI = _permutationArray[_i + 9];
		    result = _permutationArray[total];  
		    _j = (_j + tmpI) & MASK_8;

		    output_array[_i + 7] = result;

		    tmpJ = _permutationArray[_j];
		    _permutationArray[_j] = tmpI;
		    _permutationArray[_i + 9] = tmpJ;		  
		    total = tmpI + tmpJ;
		    //Round 10
		    tmpI = _permutationArray[_i + 10];
		    result = _permutationArray[total];  
		    _j = (_j + tmpI) & MASK_8;

		    output_array[_i + 8] = result;

		    tmpJ = _permutationArray[_j];
		    _permutationArray[_j] = tmpI;
		    _permutationArray[_i + 10] = tmpJ;		  
		    total = tmpI + tmpJ;
		    //Round 11
		    tmpI = _permutationArray[_i + 11];
		    result = _permutationArray[total];  
		    _j = (_j + tmpI) & MASK_8;

		    output_array[_i + 9] = result;

		    tmpJ = _permutationArray[_j];
		    _permutationArray[_j] = tmpI;
		    _permutationArray[_i + 11] = tmpJ;		  
		    total = tmpI + tmpJ;
		    //Round 12
		    tmpI = _permutationArray[_i + 12];
		    result = _permutationArray[total];  
		    _j = (_j + tmpI) & MASK_8;

		    output_array[_i + 10] = result;

		    tmpJ = _permutationArray[_j];
		    _permutationArray[_j] = tmpI;
		    _permutationArray[_i + 12] = tmpJ;		  
		    total = tmpI + tmpJ;
		    //Round 13
		    tmpI = _permutationArray[_i + 13];
		    result = _permutationArray[total];  
		    _j = (_j + tmpI) & MASK_8;

		    output_array[_i + 11] = result;

		    tmpJ = _permutationArray[_j];
		    _permutationArray[_j] = tmpI;
		    _permutationArray[_i + 13] = tmpJ;		  
		    total = tmpI + tmpJ;
		    //Round 14
		    tmpI = _permutationArray[_i + 14];
		    result = _permutationArray[total];  
		    _j = (_j + tmpI) & MASK_8;

		    output_array[_i + 12] = result;

		    tmpJ = _permutationArray[_j];
		    _permutationArray[_j] = tmpI;
		    _permutationArray[_i + 14] = tmpJ;		  
		    total = tmpI + tmpJ;
		    //Round 15
		    tmpI = _permutationArray[_i + 15];
		    result = _permutationArray[total];  
		    _j = (_j + tmpI) & MASK_8;

		    output_array[_i + 13] = result;

		    tmpJ = _permutationArray[_j];
		    _permutationArray[_j] = tmpI;
		    _permutationArray[_i + 15] = tmpJ;		  
		    total = tmpI + tmpJ;
		    //Round 16
		    tmpI = _permutationArray[ (_i + 16)];
		    result = _permutationArray[total];  
		    _j = (_j + tmpI) & MASK_8;

		    output_array[_i + 14] = result;

		    tmpJ = _permutationArray[_j];
		    _permutationArray[_j] = tmpI;
		    _permutationArray[(_i + 16 )] = tmpJ;		  
		    total = tmpI + tmpJ;

		    tmpI = _permutationArray[ (_i + 17)];
		    result = _permutationArray[total];  
		    _j = (_j + tmpI) & MASK_8;
		    output_array[_i + 15] = result; //final allocation of loop
	} //end of loop

	//241st - 257th round
	    // Round 241
	    //done in the loop above tmpI = _permutationArray[241]; //lookup, _i = 241
	    // done in the loop above _j = (_j + tmpI) & MASK_8;
	
	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[241] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 242
	    tmpI = _permutationArray[242];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[240] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[242] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 243
	    tmpI = _permutationArray[243];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[241] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[243] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 244
	    tmpI = _permutationArray[244];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[242] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[244] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 245
	    tmpI = _permutationArray[245];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[243] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[245] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 246
	    tmpI = _permutationArray[246];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[244] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[246] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 247
	    tmpI = _permutationArray[247];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[245] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[247] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 248
	    tmpI = _permutationArray[248];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[246] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[248] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 249
	    tmpI = _permutationArray[249];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[247] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[249] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 250
	    tmpI = _permutationArray[250];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[248] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[250] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 251
	    tmpI = _permutationArray[251];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[249] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[251] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 252
	    tmpI = _permutationArray[252];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[250] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[252] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 253
	    tmpI = _permutationArray[253];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[251] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[253] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 254
	    tmpI = _permutationArray[254];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[252] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[254] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 255
	    tmpI = _permutationArray[255];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[253] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[255] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 256
	    tmpI = _permutationArray[0];  // _i has wrapped around!!
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[254] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[0] = tmpJ;		  
	    total = tmpI + tmpJ;
	    //Round 257
	    tmpI = _permutationArray[1];
	    result = _permutationArray[total];  
	    _j = (_j + tmpI) & MASK_8;

	    output_array[255] = result;

	    tmpJ = _permutationArray[_j];
	    _permutationArray[_j] = tmpI;
	    _permutationArray[1] = tmpJ;		  
	    total = tmpI + tmpJ;

	    _i = 1; 		//ensure _i is correct

	    result = _permutationArray[total];  
	    output_array[256] = result; //final allocation of loop
		
      }//end of 257 output method
};


