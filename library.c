//#include <time.h>
#include "library.h"
#include "createText.h"

int CreateChamber(int amountOfChambers)
{
	if(amountOfChambers < 1)return 0;
	srand(time(NULL));
	int result;
	//create chamber
	for(int a=0; a < amountOfChambers; a++){
		CHAMBER* chamber = malloc(sizeof(CHAMBER));
		memset(chamber, 0, sizeof(CHAMBER));
		
		if (chamber == NULL) { printf("ERROR: not enough memory\n");return 0; }
		chamber->chamberID = CountChambers() + 1;
		chamber->totalBooksInChamber = 0;
		//create bookcases/walls
		for(int b=0; b < 4 ; b++){
			chamber->bookcase[b].bookcaseID = b+1;
			//create shelves
			for(int c=0; c < 5; c++){
				chamber->bookcase[b].shelves[c].shelfID = c+1;
				//create books
				for(int d =0; d < 12; d++){
					chamber->bookcase[b].shelves[c].books[d].booknumber = d+1;
					chamber->bookcase[b].shelves[c].books[d].amountOfPages = 100;
					strcpy(chamber->bookcase[b].shelves[c].books[d].author, "LibraryRNG");
					//calloc instead of malloc sets all bytes to 0 first
					char* pBuf = calloc(sizeof(chamber->bookcase[b].shelves[c].books[d].title)+1, sizeof(char));
					CreateText(titleLength, pBuf);
					strcpy(chamber->bookcase[b].shelves[c].books[d].title, pBuf);
					free(pBuf);
					pBuf = NULL;
					//a book without pages has been created.
					chamber->totalBooksInChamber += 1;
					//create 100 pages for each book
					for(int e=0; e < 100; e++){
						chamber->bookcase[b].shelves[c].books[d].bookPages[e].pageNumber = e+1;
						char* pBuf2 = calloc(sizeof(chamber->bookcase[b].shelves[c].books[d].bookPages[e].text)+1, sizeof(char));
						CreateText(lettersPerPage, pBuf2);
						strcpy(chamber->bookcase[b].shelves[c].books[d].bookPages[e].text, pBuf2);
						free(pBuf2);
						pBuf2 = NULL;
					}
				}
			}
		}
		FILE* libraryFile = NULL;
		libraryFile = fopen("Library.dat", "ab"); //append to 1 big file, this way its easy to expand the library
		result = SaveChamber(chamber, libraryFile);
		free(chamber);
		fclose(libraryFile);
		chamber = NULL;

	}
	if(result > 0){
		return 1;
	}
	else return result;
}
///the mode of the stream must be ab!
int SaveChamber(CHAMBER* chamberPtr, FILE* libraryFile){
	if(chamberPtr == NULL)return -1;
	if(libraryFile == NULL)return -2;
	if(fwrite(chamberPtr, sizeof(CHAMBER), 1, libraryFile)){
		printf("Created chamber: #%d\n", chamberPtr->chamberID);
		return 1;
	}
	return 0;
}
int CountChambers(){
	FILE* libraryFile = fopen("Library.dat", "rb");
	if(libraryFile == NULL)return 0;
	fseek(libraryFile, 0L, SEEK_END); //sets position to end of file
	int chambers = ftell(libraryFile); //returns the size
	fclose(libraryFile);
	chambers /= sizeof(CHAMBER); //filesize : the size of the struct = the amount of chambers
	return chambers;
}
///the mode of the stream must be rb!
int LoadChamber(int chamberNR, CHAMBER* chamberPtr, FILE* libraryFile){
	if(libraryFile == NULL)return -1;
	if(chamberNR < 1)return -2;
	if(chamberNR > CountChambers())return -3;
	chamberNR -= 1; // chamber 1 is element 0
	if(fseek(libraryFile, sizeof(CHAMBER)*chamberNR, SEEK_SET) == 0){
		if(fread(chamberPtr, sizeof(CHAMBER), 1, libraryFile)) { //if its 0 nothing is read - failed
			return 1;
		}
	}
	return 0;
}
//returns 1 if word is found
int SearchLibrary(CHAMBER* chamber,FILE* libraryFile, LIST* list, char* word)
{
	if(chamber == NULL)return -1;
	if(libraryFile == NULL)return -2;
	if(word[0] == '\0')return -3; //means string is empty
	int location = (ftell(libraryFile)+sizeof(CHAMBER))/sizeof(CHAMBER);
	if(LoadChamber(location, chamber, libraryFile) == 1)
	{
		//foreach bookcase
		for(int a=0; a < 4 ; a++){
			//foreach shelf
			for(int b=0; b < 5; b++){
				//foreach book
				for(int c =0; c < 12; c++){
					//foreach pagelace
					for(int d=0; d < 100; d++){
						if(strstr(chamber->bookcase[a].shelves[b].books[c].bookPages[d].text, word) != NULL)
						{
							//set the pointer to the next item in the list, until last item (NULL) is reached.
							while (list->next != NULL)
							{
								list = list->next;
							}
							//pointer is set to last node
							//fill in variables for current node and create new empty node
							list->next = malloc(sizeof(LIST));
							if(list->next == NULL)return -4;
							list->next->place.bookcase = a;
							list->next->place.shelf = b;
							list->next->place.book = c;
							list->next->place.page = d;
							list->next->place.chamber = location;
							//create new empty node
							list->next->next = NULL;
						}
					}
				}
			}
		}
	}
	return location;
}

/* COMPILE WITH: gcc -Wall -Wextra -pedantic -std=c99 -g main.c library.c createText.c -o main
 * compile with -g
 * crashed: gdb <application_name> core
 * backtrace
 *
 * USED SOURCES:
 * - https://stackoverflow.com
 * - http://www.learn-c.org/en/Linked_lists
 * - http://www.learn-c.org/en/Dynamic_allocation
 * - http://www.learn-c.org/en/Structures
 * 
 * Valgrind errors: 
 * 
 * Syscall param write(buf) points to uninitialised byte(s):
 * Due to data structure alignment, some value address may be forced to align to address multiple word size. 
 * Therefore, several unused bytes is padded between BOOK::title and BOOK::bookNumber. 
 * Those are the bytes which weren't initialized and thus reported by Valgrind. (calloc instead of malloc solves this.)
 * 
 * Invalid read of size 1: 
 * This is most likely an array out of bound error, when you strcpy it copies a termination character at the end of the char array
 * if you didnt allocate memory for this (extra)character, you will get this error in valgrind. (sizeof(data)+1)
 * 
 * Conditional jump or move depends on uninitialised value: 
 * some values in the struct were uninialized. easy fix is to set all values to 0 with memset.
 * */
