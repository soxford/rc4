/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: RC4Stream.cpp
 * Author: Simon Campbell, <simonhmcampbell@gmail.com>
 * Description: code for the rc4 stream cipher
 * License: GPL
 * Date: April 2015
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//TODO complete encapsulation of this class and then complete modification of driver file
#include <cstdint>

class RC4Stream
{
   private:
      uint8_t* _permutationArray; //the permutation array which must have length PERMUTATION_ARRAY_LENGTH and contain one of each byte value < this length
      int _i; // the first internal state which increments on each output, 0 <= i < PERMUTATION_ARRAY_LENGTH
      int _j; // the second internal state which defined by RC4

   public:
      static const int PERMUTATION_ARRAY_LENGTH = 256; //permutation array length (no of bytes), 256 is used in TLS

      //constructor
      RC4Stream() : _permutationArray(new uint8_t[PERMUTATION_ARRAY_LENGTH]), _i(0), _j(0) {
      }

      //destructor
      ~RC4Stream() {
         delete[] _permutationArray;
      }
     
      //Key class which instantiates the rc4 key 
      class Key {
         private:
            uint8_t* _key;

         public:
            static const int KEY_LENGTH = 16; //_key length (no of bytes), 16 is used in TLS
            
            Key() : _key(new uint8_t[KEY_LENGTH]) {}

            ~Key() {delete[] _key; }
            
            //method returns the appropriate byte of the _key, modulo the KEY_LENGTH
            uint8_t getModuloLength(int i) {
               i = i % KEY_LENGTH;
               return (i > -1 ? _key[i] : _key[i + KEY_LENGTH]);
            }
            
            //Method to set the ith (modulo KEY_LENGTH) byte
            void setModuloLength( int i, uint8_t byte ) {
               i = i % KEY_LENGTH;
               i = (i > -1 ? i : i + KEY_LENGTH);
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
                  virtual void selectRandomKey(RC4Stream::Key *key) {}
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
       void keySchedule(RC4Stream::Key *key){

         //initialize the permutation array to be the identity permutation
         for (int i = 0; i < PERMUTATION_ARRAY_LENGTH ; i++) {
            _permutationArray[i] = i;
         }
         
         //schedule the permutation array
         unsigned int j = 0;
         for (int i = 0; i < PERMUTATION_ARRAY_LENGTH; i++) {
            j = (j + (unsigned int) _permutationArray[i] + (unsigned int) key->getModuloLength(i)) % PERMUTATION_ARRAY_LENGTH;
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
      uint8_t PRGRound()
      {
            _i = (_i + 1) % PERMUTATION_ARRAY_LENGTH;
            _j = (_j + _permutationArray[_i]) % PERMUTATION_ARRAY_LENGTH;

            //swap ith and jth elements
            uint8_t tmp = _permutationArray[_i];
            _permutationArray[_i] = _permutationArray[_j];
            _permutationArray[_j] = tmp;  
            
            //return the output
            return _permutationArray[(_permutationArray[_i] + _permutationArray[_j]) % PERMUTATION_ARRAY_LENGTH];  
      }
};


