#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

char validLetters[] = "ADEFNORRSSW"; //Char array for the valid letters
int validLettersdistro[26]; //Declaring a 26 int array to store the distribution for the valid letters


//Function Initializations
int initialization();
void gameLoop();
void teardown();
bool isDone();
void displayWorld();
void acceptInput();
void make_upper(char *string);
void strip_string(char *string);
void getLetterDistribution(char *input, int *distro);
bool compareCounts(int *candidate_word_distro, int *valid_words_distro);
bool input_checker(char *string);

//Main function
int main()
{
    initialization(); //Call to the initialization function
    gameLoop(); //Call to the game loop function
    teardown(); //Call to the teardown function
    return 0;
}

int initialization()
{
    srand(time(NULL)); //Getting a random int
    return 0; //returning 0(for now)
}

void gameLoop()
{
    make_upper(validLetters); //Making sure the valid letters are uppercase
    getLetterDistribution(validLetters, validLettersdistro); //Getting the distribution for the valid letters
    do
    {
        displayWorld(); //Call to the display world function
        acceptInput(); //Call to the accept input function
    }while(!isDone()); //Do while the isdone function returns false
}

/*
 * teardown function
 * simply prints "All Done"
 */
void teardown()
{
    printf("All Done\n");
}

/*
 *isDone function
 *Only returns true for now
 */
bool isDone()
{
    return true;
}

void displayWorld()
{
    //Printing the valid letters
    for(int i = 0; i < strlen(validLetters); i++) {
        printf("%c ", validLetters[i]);
    }
    //Displaying a line between the valid letters and the playing space
    printf("\n---------------------\n");
}

void acceptInput()
{
    int distro[26]; //declaring a 26 int array for the guess distribution

    char buffer[20]; //declaring a 20 char array for the input from the user

    printf("Enter a guess: "); //Prompting the user

    fgets(buffer, sizeof(buffer), stdin); //Reading the user input from the keyboard

    //Checking if the input from the user only has letters
    if(input_checker(buffer)) {
        //If the input is valid make the input only uppercase and remove the newline character
        make_upper(buffer);
        strip_string(buffer);

        //Getting the guess distribution
        getLetterDistribution(buffer, distro);

        //Checking if you can make the guess using the valid letters
        if(compareCounts(distro, validLettersdistro)) {
            printf("You win!\n"); //If true display that the user has "won" or found a valid word

        } else {
            printf("You lose!\n"); //Otherwise display that the user has "lost" or guessed a word that is not using the valid letters
        }
    }
    //If the input is not valid, display to the user that the input is not valid
    else {
        printf("Wrong input!\n");
    }
}

/*Helper functions
 *make_upper: Takes a pointer to a char array as input and makes the string uppercase
 *
 *strip_string: Takes a pointer to a char array as input and removes the newlines
 *
 *getLetterDistribution: Takes 2 pointers, one to a char array and one to a int array. makes a distribution in the int array
 *based on the letters of the char array(ie: "aa" -> [2,0,...]
 *
 *compareCounts: Takes 2 pointers, both to letter distributions found in the previous function. checks each letter against each other
 *making sure the candidate can be made using the words in the master word
 *
 *input_checker: Makes sure the user only put in alphabetical characters
 */


void make_upper(char *string)
{
    int length = strlen(string);
    for(int i = 0; i < length; i++)
    {
        string[i] = toupper(string[i]); //Iterating through the char array, making each character uppercase
    }
}

void strip_string(char *string)
{
    //Removing the newline character
    string[strlen(string) - 1] = '\0';
}

void getLetterDistribution(char *input, int *distro)
{
    //Initializing the int array to all 0
    for(int i = 0; i < 26; i++)
    {
        distro[i] = 0;
    }
    for(int i = 0; i < strlen(input); i++)
    {
        //Using ascii to find the correct position for the character
        //Ex) A-A = 0 index: 0
        //B-A = 1 index: 1
        int index = input[i] - 'A';
        distro[index]++; //Iterating the value at the index of the letter
    }
}

bool compareCounts(int *candidate_word_distro, int *valid_words_distro)
{
    for(int i = 0; i < 26; i++)
    {
        //Checking if the guess has a higher distribution of any letter than the valid letters
        if(candidate_word_distro[i] > valid_words_distro[i])
        {
            //If it does then it is not a valid guess
            return false;
        }
    }
    //otherwise return true
    return true;
}

bool input_checker(char *string) {
    int length = strlen(string);
    for(int i = 0; i < length-1; i++) {
        //Checking if each character is a letter
        if(!isalpha(string[i])) {
            //If finding a non letter character then return false
            return false;
        }
    }
    //Otherwise return true
    return true;
}
