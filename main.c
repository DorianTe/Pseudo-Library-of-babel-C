#include "library.h"

unsigned stayInLibrary = 1;
char buf[20];

enum STATE{
	inChamber =1,
	inBookcase,
	inShelf,
	inBook,
	inPage,
	changeChamber
};

void InputHandler(char input[])
{
	do {
		//maximum input length = 20
		fgets(input, 20, stdin);
	}
	while(input[0] < 0 || input[0] == '\n');
}
//tried to make this a universal input handler...
int CheckValidInput()
{
	char input[20];
	memset(input, 0, sizeof(&input));
	InputHandler(input);
	int number;
	if(sscanf(input, "%d", &number) > 0)
	{
		return number;
	}
	else
	{
		if(strcmp("q\n", &input[0]) == 0)
		{
			return -1;
		}
		if(strcmp("^\n", &input[0]) == 0)
		{
			return -2;
		}
		if(strcmp("<<\n", &input[0]) == 0)
		{
			return -3;
		}
		if(strcmp(">>\n", &input[0]) == 0)
		{
			return -4;
		}
		if(strcmp("S\n", &input[0]) == 0)
		{
			return -5;
		}
		if(input[0] == '#')
		{
			memset(buf, 0, sizeof(buf));
			char* stripped = input;
			stripped++;//remove #
			stripped[strlen(stripped) -1] = '\0';//remove '\n'
			strcpy(buf, stripped);
			return -6;
		}
		if(strcmp("y\n", &input[0]) == 0)
		{
			return -7;
		}
	}
	return -99;
}
void StateChanger(CURRENT* place, int value){
	if(place != NULL) {
		switch (value) {
			case -1:
				place->state = 0;
				break;
			case -2:
				place->state -= 1;
				break;
			case -3:
				place->chamber -= 1;
				place->state = changeChamber;
				break;
			case -4:
				place->chamber += 1;
				place->state = changeChamber;
				break;
			case -5: place->state = 0; stayInLibrary = 0;
				break;
			default:
				break;
		}
	}
}

int WalkThroughLibrary()
{
	short amount;
	if((amount = CountChambers()) == 0)
	{
		printf("There are no chambers\n");
		return 0;
	}
	printf("There are a total of %d chambers. \nIn which chamber do you wish to spawn?", amount);
	int startChamber = CheckValidInput();
	if(startChamber < 1){printf("Seriously? you want to enter chamber 0? What are you? A programmer, pff?"); return WalkThroughLibrary();}
	if(startChamber > amount){printf("There are only %d chamber(s)\n", amount); return WalkThroughLibrary();}
	printf("Welcome to the library\nYou can always enter the following expressions upon input:\n"
				   " - Entering 'q' exits the library and brings you back to the main menu. \n"
				   " - << or >> tells me you want to go to the next or previous chamber.\n"
				   " - '^' tells me you want to go up a level (so if you're in a shelf, it brings you back to the bookcase and thus asks you for a shelf)\n"
				   " - 'S' to stop the program completely\n");

	CURRENT place;
	CHAMBER* chamber = malloc(sizeof(CHAMBER));
	FILE* libraryFile = fopen("Library.dat", "rb");
	int checkValue;
	place.chamber = startChamber;
	place.state = changeChamber;

	while(place.state != 0){
		switch(place.state)
		{
			case inChamber:
				printf("\t\t\t~You have entered chamber: %d ~\n", chamber->chamberID);
				printf("You see 4 bookcases, which bookcase do you wish to inspect?");
				if((checkValue = CheckValidInput()) > 0 && checkValue <= 4){
					place.bookcase = checkValue-1;
					printf("You inspect bookcase: %d \n", chamber->bookcase[place.bookcase].bookcaseID);
					place.state = inBookcase;
					StateChanger(&place, checkValue);
				}
				else if(checkValue < 0 && checkValue > -6){
					StateChanger(&place, checkValue);
				}
				break;
			case inBookcase:
				printf("You see 5 shelves, which shelf do you wish to inspect?");
				if((checkValue = CheckValidInput()) > 0 && checkValue <= 5) {
					place.shelf = checkValue-1;
					printf("You inspect shelf: %d \n", chamber->bookcase[place.bookcase].shelves[place.shelf].shelfID);
					place.state = inShelf;
				}
				else if(checkValue < 0 && checkValue > -6){
					StateChanger(&place, checkValue);
				}
				break;
			case inShelf:
				printf("You see 12 books, which book do you wish to inspect?");
				if((checkValue = CheckValidInput()) > 0 && checkValue <=12) {
					place.book = checkValue-1;
					printf("You inspect book: %d, its title is: %s \n",
						   chamber->bookcase[place.bookcase].shelves[place.shelf].books[place.book].booknumber,
						   chamber->bookcase[place.bookcase].shelves[place.shelf].books[place.book].title);
					place.state = inBook;
				}
				else if(checkValue < 0 && checkValue > -6){
					StateChanger(&place, checkValue);
				}
				break;
			case inBook:
				printf("This book has 100 pages, which page do you wish to open?");
				if((checkValue = CheckValidInput()) > 0 && checkValue <= 100) {
					place.page = checkValue - 1;
					printf("\t\t\t ~You opened page: %d ~\n  ",
						   chamber->bookcase[place.bookcase].shelves[place.shelf].books[place.book].bookPages[place.page].pageNumber);
					place.state = inPage;
				}
				else if(checkValue < 0 && checkValue > -6){
					StateChanger(&place, checkValue);
				}
				break;
			case inPage:
				printf("%s\n", chamber->bookcase[place.bookcase].shelves[place.shelf].books[place.book].bookPages[place.page].text);
				if((checkValue = CheckValidInput()) < 0)StateChanger(&place, checkValue);
				break;
			case changeChamber:
					while((checkValue = LoadChamber(place.chamber, chamber, libraryFile)) < 1)
					{
						switch (checkValue)
						{
							case -1:printf("file NULL");
								break;
							case -2:printf("There are no chambers below chamber 1\n");place.chamber += 1;
								break;
							case -3: printf("There are no more chambers\n");place.chamber -= 1;
								break;
						}
					}
				place.state = inChamber;
				break;
			default: printf("Quitted library");
				break;
		}
	}
	free(chamber);
	chamber = NULL;
	fclose(libraryFile);
	return 1;
}
int WordSearcher() {
		printf("Welcome to the word searcher! Type a word to search the library, start the word with a #:");
		if(CheckValidInput() != -6){printf("Wrong input\n"); return WordSearcher();}
		CHAMBER* chamber = malloc(sizeof(CHAMBER));
		if (chamber == NULL) {
			printf("NoMemory\n");
			return -1;
		}
		FILE* libraryFile;
		if ((libraryFile = fopen("Library.dat", "rb")) == NULL) {
			printf("cant find file\n");
			return -2;
		}
		char* word = malloc(sizeof(buf));
		if (word == NULL) {
			printf("NoMemory\n");
			return -3;
		}
		strcpy(word, buf);

		LIST* list = malloc(sizeof(LIST));
		memset(list, 0, sizeof(LIST));
		if (list == NULL) {
			printf("NoMemory\n");
			return -4;
		}
		int amountOfChambers = CountChambers();
		int scanned = 1;
		printf("scanned chamber: ");
		while (SearchLibrary(chamber, libraryFile, list, word) <= amountOfChambers) {
			printf("%d, ", scanned++);
		}
		printf("\n");
		fclose(libraryFile);
		//display found elements
		if(list->next == NULL){printf("no results\n");}
		while (list != NULL) {
			if (list->place.chamber > 0) {
				printf("Word found on page %d book %d from shelf %d from bookcase %d from chamber %d\n",
					   chamber->bookcase[list->place.bookcase].shelves[list->place.shelf].books[list->place.book].bookPages[list->place.page].pageNumber,
					   chamber->bookcase[list->place.bookcase].shelves[list->place.shelf].books[list->place.book].booknumber,
					   chamber->bookcase[list->place.bookcase].shelves[list->place.shelf].shelfID,
					   chamber->bookcase[list->place.bookcase].bookcaseID, list->place.chamber);
			}
			list = list->next;
		}

		free(list);
		free(chamber);
		free(word);
		list = NULL;
		chamber = NULL;
		word = NULL;
		printf("Would you like to search for more words? y = yes, n = no\n");
		if(CheckValidInput() == -7)return WordSearcher();
		return 1;
}
void mainMenu()
{
	printf("\nOptions:\n");
	printf("\n [0] Create chamber(s) \n [1] Walk through library\n [2] Search for a word in the library\n");
	int i = CheckValidInput();
	//had to use function pointers somewhere, although not very useful in this case, a usefull case would be for a callback function
	int (*funcWalk)(void);
	funcWalk = WalkThroughLibrary;

	switch(i)
	{
		case 0:
			printf("\nHow many chambers should be created?");
			if(CreateChamber(CheckValidInput()) < 1)
			{
				printf("Seriously? you can't create 0 chambers or less...");
			}
			break;
		case 1:
			funcWalk();
			break;
		case 2:
			WordSearcher();
			break;
	}
}
int main()
{
	printf("\t\t\t ~~ Welcome to The Library of Babel ~~ \n");
	while(stayInLibrary){
		mainMenu();
	}
	return 0;
}


