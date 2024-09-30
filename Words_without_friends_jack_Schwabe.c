#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

int initialization();
void gameLoop();
void teardown();
bool isDone();
void displayWorld();
void acceptInput();
void make_upper(char *string);
void strip_string(char *string);
void getLetterDistribution(char *input, int *distro);
bool compareCounts(int *candidate_word, int *options);

int main()
{
	initialization();
	gameLoop();
	teardown();
	return 0;
}

int initialization()
{
	srand(time(NULL));
	return 0;
}

void gameLoop()
{
	do
	{
		displayWorld();
		acceptInput();
	}while(!isDone());
}

void teardown()
{
	printf("All Done\n");
}

bool isDone()
{
	return true;
}

void displayWorld()
{
	printf("---------------------\n");
}

void acceptInput()
{
    int distro[26];
	char buffer[20];
	printf("Enter a guess: ");
	fgets(buffer, sizeof(buffer), stdin);
	make_upper(buffer);
	strip_string(buffer);
	getLetterDistribution(buffer, distro);
}


void make_upper(char *string)
{
	int length = strlen(string);
	for(int i = 0; i < length; i++)
	{
		string[i] = toupper(string[i]);
	}
}

void strip_string(char *string)
{
	string[strlen(string) - 1] = '\0';
}

void getLetterDistribution(char *input, int *distro)
{
	for(int i = 0; i < 26; i++)
	{
		distro[i] = 0;
	}

	for(int i = 0; i < strlen(input); i++)
	{
		int index = input[i] - 'A';
		distro[index]++;
	}

	for(int k = 0; k < 26; k++)
	{
		printf("the distro at index: %d is %d\n", k , distro[k]);
	}
}

bool compareCounts(int *candidate_word_distro, int *valid_words_distro)
{
	for(int i = 0; i < 26; i++)
	{
		if(candidate_word_distro[i] > valid_words_distro[i])
		{
			return false;
		}
	}
	return true;
}
