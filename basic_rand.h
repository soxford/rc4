/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: basic_rand.h
 * Author: Simon Campbell, <simonhmcampbell@gmail.com>
 * Description: Defines random initialization and getRand functions using the standard rand function
 * License: GPL
 * Date: April 2015  
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//Initializes the basic random number generator
void initializeRandomNoGen(){
   srand(time(NULL));
}

//provides the next random byte
uint8_t getRand(){
   return (uint8_t) (rand() % 256);
}
