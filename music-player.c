// This Program stores Music File details,
// and will launch an external application to play
// a file when the user selects a preference.
// By Andrew Colbeck 2017

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum album_genre{ rock=1, pop, alternative, drum_and_bass} album_genre;

//---------------------------------------------------
//					**** STRUCTURES ****			
//---------------------------------------------------
typedef struct string
{
	char str[256];
}string;

typedef struct album
{
	char album_name[100];
	album_genre genre;
	int track_number;
	string tracks[10];
	string tracklocation;
}album;

typedef struct album_array
{
	int size;
	album *ptr;
}album_array;

//---------------------------------------------------
//					**** FUNCTIONS ****			
//---------------------------------------------------

album_genre read_genre()
{
	//This function reads in a value for enum of music genres
	//passing back an enum value (selection)
	album_genre selection=0;
	
	do
	{
		printf("Enter 1 = Rock, 2 = Pop, 3 = Alternative, 4 = Drum and Bass\n");
		scanf("%i", &selection);

		if (selection >= 1 && selection <=4)
			break;

		printf("Please enter a number between 1 - 4 only.\n");
		selection = 0;
	}while (selection == 0);

	return selection;
}

album get_album()
{
	//Function to read in Album
	int i;
	album temp;

	printf("Enter Album Name:\n");
	scanf("%s", temp.album_name);
	printf("Enter Album Genre:\n");
	temp.genre=read_genre();
	printf("Enter Number of Tracks:\n");
	scanf("%i", &temp.track_number);

	for (i = 0; i < temp.track_number; i++)
	{
		printf("Enter Track %i Title:", i+1);
			//i+1 increments track number from 0 to 1
		scanf("%s", temp.tracks[i].str);
	}

	printf("Please enter the File location of Tracks:\n");
	scanf("%s", temp.tracklocation.str);
	return temp;
}

void add_album(album_array *aa)
{
	//Function to create a new memory location
	//and store the data of an Album (in an array of albums)
	album *new_album;
		//Declare a Pointer of type album (struct)
	
	aa->size++;
		//Increment the size stored in the new element

	new_album=(album*)realloc(aa->ptr,aa->size*sizeof(album));
		//Assign the pointer to the newly declared sample, reallocate a new memory block for the next element

	if(new_album)
	{
		aa->ptr=new_album;
		//Return Pointer to previous memory block, and begin storing data with get_album function:
		aa->ptr[aa->size-1]=get_album();
	}
	else{
		printf("Out of memmory!\n");
		aa->size--;
	}
}


void print_album(album a)
{
	//Prints out an album's details
	int i;

	printf("Name of the Album:\t%s\n", a.album_name);
	printf("Genre of the Album:\t");

	switch (a.genre)
	//Reads the enum value stored in a.genre
	{
		case rock: printf("Rock\n");
			break;
		case pop: printf("Pop\n");
			break;
		case alternative: printf("Alternative\n");
			break;
		case drum_and_bass: printf("Drum and Bass\n");
			break;	
	}

	printf("No. of Tracks:\t\t%i\n\n", a.track_number);

	for (i = 0; i < a.track_number; i++)
		printf("%i. %s\n", i+1, a.tracks[i].str);

	printf("\nLocated in %s Folder\n", a.tracklocation.str);
}


void select_track_to_play(album_array *aa)
{
	//Allows the user to choose an album 
	//and then a track to play
	int i, selection=0, tselect=0;

	char command[256+1];
		//Declare a character array to store file path for calling with system() function

	printf("\nWhich Album would you like to Browse?\n\n");

	for (i = 0; i < aa->size; i++)
		printf("%i. %s\t", i+1, aa->ptr[i].album_name);
	printf("\n");
	scanf("%i", &selection);
	selection-=1;
		//Subtracting 1 to selection compensates for Array starting at 0

	printf("Select a Track:\n");

	for (i = 0; i < aa->ptr[selection].track_number; i++)
		printf("%i. %s\n", i+1, aa->ptr[selection].tracks[i].str);

	scanf("%i", &tselect);
	tselect-=1;
		//See note above
	printf("The Track you selected, %s ", aa->ptr[selection].tracks[tselect].str);


	printf("from Album %s ", aa->ptr[selection].album_name);
	printf("\nis now playing from file location: %s.  Enjoy!\n\n", aa->ptr[selection].tracklocation.str);

  	sprintf(command, "open %s/%s.mp3", aa->ptr[selection].tracklocation.str, aa->ptr[selection].tracks[tselect].str);

	system("open -a VLC");
	system(command);
}


//---------------------------------------------------
//					**** MAIN PROGRAM ****			
//---------------------------------------------------

int main()
{
	int option=0, i;
	album_array aa={0, NULL};

	printf("\n\n");
	printf("\t****************************************\n");
	printf("\t*                                      *\n");
	printf("\t*           Music Player v1.0          *\n");
	printf("\t*                                      *\n");
	printf("\t****************************************\n");

	while (option !=4)
	{
		printf("\t     Please select an option:\n");
		printf("\t     1. Add an Album\n");
		printf("\t     2. Print all Albums\n");
		printf("\t     3. Select a Track to Play\n");
		printf("\t     4. Quit\n");
		scanf("%d",&option);

		switch (option)
		{
			case 1: 
				//Add an Album (into album array)
				printf("Add an Album\n");
				add_album(&aa);
				option=0;
			 		break;

			case 2: 
				//Print all Albums
				for (i = 0; i<aa.size; i++){
					printf("--------------------------\n");
			 		print_album(aa.ptr[i]);
				}
				printf("--------------------------\n");
				printf("\n");
				option=0;
					break;

			case 3: 
				//Select a Track to Play
				select_track_to_play(&aa);
				option=0;
					break;

			case 4: printf("Quit Program.\n");
					break;

			default: printf("You have entered an invalid option.\n");
				option=0;
					break;
		}
	}
}