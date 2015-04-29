/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: basic_rand.h
 * Author: Simon Campbell, <simonhmcampbell@gmail.com>
 * Description: Defines random initialization and getRand functions using the standard rand function
 * License: GPL
 * Date: April 2015  
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "rc4.h"
#include <stdlib.h>
//Initializes the basic random number generator
void initializeRandomNoGen(){
   srand(time(NULL));
}

//provides the next random byte
uint8_t getRand(){
   return (uint8_t) (rand() % 256);
}

//generate the random key - assumes the key provided is of length KEY_LENGTH bytes
void selectRandomKey(uint8_t* key){
	for(int i = 0; i < KEY_LENGTH; i++){
		key[i] = getRand();
	}
}
