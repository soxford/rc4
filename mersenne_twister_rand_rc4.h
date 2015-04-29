//TODO factorise and objectify random number generation interface and implementations
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: mersenne_twister_rand_rc4.h
 * Author: Simon Campbell, <simonhmcampbell@gmail.com>
 * Description: Defines random initialization, getRand and select Random Key functions using the standard mt19937 random number generator
 * License: GPL
 * Date: April 2015  
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "rc4.h"
#include <random>



//Instantiate a mersenne twister object
std::mt19937 MT;

//Initializes the random number generator
void initializeRandomNoGen(){
	MT.seed(time(0));
}

//generate the random key - assumes the key provided is of length KEY_LENGTH bytes
void selectRandomKey(uint8_t* key){
	//get the number of required randome numbers from the twister
	int noOfRandsRequired = KEY_LENGTH/sizeof(std::mt19937::result_type) + 1;
	int randoms[noOfRandsRequired];
	for(int i = 0; i < noOfRandsRequired; i++){
		randoms[i] = MT();	
	}

	for(int i = 0; i < KEY_LENGTH; i++){
		//TODO review this assignment to confirm correctness
		key[i] = (uint8_t) ((randoms[i*sizeof(uint8_t)/sizeof(std::mt19937::result_type)] >> (sizeof(uint8_t)*(i % sizeof(std::mt19937::result_type)) )) & 0xff);
	}
}