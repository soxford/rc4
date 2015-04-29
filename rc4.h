/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: rc4
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
      static const int KEY_LENGTH = 16; //key length (no of bytes), 16 is used in TLS
   
      //constructor
      RC4Stream() : _permutationArray(new uint8_t[PERMUTATION_ARRAY_LENGTH]), _i(0), _j(0) {
      }

      //destructor
      ~RC4Stream() {
         delete[] permutationArray;
      }

      /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
       * Key Scheduling function
       * input - RC4Stream to be initialised, key to be used in initialisation
       * output - void
       * description - permutes the elements of the permutation array according to the RC4 Key Scheduling Algorithm and initialises the internal state variables
       * Assumes that the rc4 stream permutation array and the key array have been assigned!
       * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
       void keySchedule(uint8_t* key){//TODO create a class for holding RC4 Keys separate (or inside?) this class

         //initialize the permutation array to be the identity permutation
         for (int i = 0; i < PERMUTATION_ARRAY_LENGTH ; i++) {
            _permutationArray[i] = i;
         }
         
         //schedule the permutation array
         unsigned int j = 0;
         for (int i = 0; i < PERMUTATION_ARRAY_LENGTH; i++) {
            j = (j + (unsigned int) _permutationArray[i] + (unsigned int) key[i % KEY_LENGTH]) % PERMUTATION_ARRAY_LENGTH;
            //swap ith and jth elements
            uint8_t tmp = _permutationArray[i];
            _permutationArray[i] = _permutationArray[j];
            _permutationArray[j] = tmp;  
         }
         //initialize the state variables 
         _i = 0;
         _j = 0;
      }

};


//TODO move into class
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * RC4 Pseudo Random Generation Algorithm round function
 * Description: Generates the next byte of output from the given RC4 stream
 * input - an RC4Stream that has been allocated a permutation array
 * output - the next keystream byte as a uint8_t 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint8_t rc4PRGRound(RC4Stream* rc4Stream)
{
      rc4Stream->i = (rc4Stream->i + 1) % PERMUTATION_ARRAY_LENGTH;
      rc4Stream->j = (rc4Stream->j + rc4Stream->permutationArray[rc4Stream->i]) % PERMUTATION_ARRAY_LENGTH;

      //swap ith and jth elements
      uint8_t tmp = rc4Stream->permutationArray[rc4Stream->i];
      rc4Stream->permutationArray[rc4Stream->i] = rc4Stream->permutationArray[rc4Stream->j];
      rc4Stream->permutationArray[rc4Stream->j] = tmp;  
      
      //return the output
      return rc4Stream->permutationArray[(rc4Stream->permutationArray[rc4Stream->i] + rc4Stream->permutationArray[rc4Stream->j]) % PERMUTATION_ARRAY_LENGTH];  
}
