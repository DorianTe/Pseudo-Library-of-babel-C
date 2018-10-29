#include <string.h> //for memset
#include <stdlib.h>
#include <time.h>
#include "createText.h"
#define amountOfcharacters 26


int pseudoRandom(int max) {
	int divisor = RAND_MAX/(max);
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval > max);
    return retval;
}
void CreateText(int length, char* randomCharacters)
{
	char alphabet[amountOfcharacters];
	memset(&alphabet, 0, amountOfcharacters); //memset, empties the array, reset
	alphabet[amountOfcharacters] = '\0'; //string closed with null terminator	
	//create alphabet	
	for(int place=0; place < amountOfcharacters; place++){
		alphabet[place] = 'a' + place;
	}
	//generate text with given length
	for(int i = 0; i < length; i++){
		randomCharacters[i] = alphabet[pseudoRandom(amountOfcharacters)];
	}
}
