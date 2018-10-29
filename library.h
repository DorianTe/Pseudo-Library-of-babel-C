#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//GENERAL SETTINGS DEFINES
#define titleLength 15
#define authorNameLength 15
#define lettersPerPage 450
#define PagesPerBook 100
#define booksPerShelf 12
#define shelfsPerBookcase 5
#define bookcasesPerChamber 4

typedef struct {
	int pageNumber;
	char text[lettersPerPage];
}PAGE;

typedef struct {
	int booknumber;
	int amountOfPages;
	char title[titleLength];
	char author[authorNameLength];
	PAGE bookPages[PagesPerBook];
}BOOK;
typedef struct {
	int shelfID;
	BOOK books[booksPerShelf];
}SHELF;

typedef struct {
	int bookcaseID;
	SHELF shelves[shelfsPerBookcase];
}BOOKCASE;

typedef struct {
	int totalBooksInChamber;
	int chamberID;
	BOOKCASE bookcase[bookcasesPerChamber];
}CHAMBER;

typedef struct {
	unsigned int chamber;
	unsigned int bookcase;
	unsigned int shelf;
	unsigned int book;
	unsigned int page;
	unsigned short state;
}CURRENT;

typedef struct node {
	CURRENT place;
	struct node* next;
} LIST;
 //each chamber has 4 walls with bookcases
int CreateChamber(int amountOfChambers);
int SaveChamber(CHAMBER* chamberPtr, FILE* libraryFile);
int LoadChamber(int chamberNR, CHAMBER* chamberPtr, FILE* libraryFile);
int CountChambers();
int SearchLibrary(CHAMBER* chamber, FILE* libraryFile, LIST* list,char* word);

#endif
