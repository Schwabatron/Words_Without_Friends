#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

char validLetters[30]; //Char array for the valid letters
int validLettersdistro[26]; //Declaring a 26 int array to store the distribution for the valid letters
int word_counter = 0;

typedef struct wordListNode  {
    char word[30]; //30 char string
    struct wordListNode *next; //pointer to next node
} wordListNode;

typedef struct gameListNode {
    char word[30];
    bool already_found;
    struct gameListNode *next;
} gameListNode;

//Compare function used for qsort
int compare(const void *a, const void *b) {
    return (*(const char *)a) - (*(const char *)b);
}
/*
If value_a is less than value_b, returns a negative.
If value_a is equal to value_b, returns zero.
If value_a is greater than value_b, returns a positive.
 */



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
void insertIntoDictionary(wordListNode **root, char *new_word);
wordListNode *getRandomWord();
void findWords(wordListNode *masterword);
void cleanupWordListNodes(wordListNode **root);
void cleanupGameListNodes(gameListNode **game_root);


//Declaring the two roots of the linked lists as global variables
wordListNode *root = NULL;
gameListNode *game_root = NULL;

//Main function
int main()
{
    initialization(); //Call to the initialization funct
    wordListNode *longWordNode = getRandomWord();
    findWords(longWordNode);
   // printf("%s\n", longWordNode->word);

   // Display the words in the game list (testing purposes)
    //gameListNode *gameCurrent = game_root;
    //printf("Words that can be formed from the master word:\n");
   // while (gameCurrent != NULL) {
    //   printf("%s\n", gameCurrent->word);
    //   gameCurrent = gameCurrent->next;
    //}

    gameLoop(); //Call to the game loop function
    teardown(); //Call to the teardown function
    return 0;
}

int initialization()
{
    srand(time(NULL)); //Setting a random time seed

    FILE *file_pointer;
    file_pointer = fopen("2of12.txt", "r");
    if (file_pointer != NULL)
    {
        char word[30];
        while((fscanf(file_pointer, "%s", word) != EOF))
        {
            insertIntoDictionary(&root, word); //Insert the word into the linked list
            word_counter++; //Increment word count
        }
        fclose(file_pointer); //Closing file
    }
    else {
        printf("File not found\n");
    }
    return word_counter; //Returning the number of words in the linked list
}

void gameLoop()
{
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
    printf("Congrats you win!!\n");
    cleanupGameListNodes(&game_root); //Cleaning up the linked lists in the teardown function
    cleanupWordListNodes(&root);
}

/*
 *isDone function
 *Only returns true for now
 */
bool isDone()
{
    //Loops through the game list and if it finds a word with already found == false it will return false
    bool done = true;
    gameListNode *gameCurrent = game_root;
    while(gameCurrent != NULL) {
        if(gameCurrent->already_found == false) {
            done = false;
        }
        gameCurrent = gameCurrent->next;
    }

    return done;
}

void displayWorld()
{
    char validletterssorted[30];
    strcpy(validletterssorted, validLetters);
    //Printing the valid letters
    qsort(validletterssorted, strlen(validletterssorted), sizeof(char), compare);
    for(int i = 0; i < strlen(validletterssorted); i++) {
        printf("%c ", validletterssorted[i]);
    }
    //Displaying a line between the valid letters and the playing space

    printf("\n---------------------\n");

    gameListNode *gameCurrent = game_root;
    while(gameCurrent != NULL) {
        if(gameCurrent->already_found) {
            printf("Found:%s\n", gameCurrent->word);
        }
        else {
            printf("Not Found:");
            for(int i = 0; i < strlen(gameCurrent->word); i++) {
                printf("_");
            }
            printf("\n");
        }
        gameCurrent = gameCurrent->next;
    }



}

void acceptInput()
{
    int distro[26]; //declaring a 26 int array for the guess distribution

    char buffer[30]; //declaring a 20 char array for the input from the user

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
        if(compareCounts(distro, validLettersdistro))
        {
            gameListNode *gameCurrent = game_root;
            bool found = false; //Checking if we found a word
            while(gameCurrent != NULL)
            {
                if(strcmp(buffer, gameCurrent->word) == 0)
                {
                    found = true;
                    if(gameCurrent->already_found)
                    {
                        printf("You already found this word! Please try again :)\n");
                    } else
                    {
                        gameCurrent->already_found = true;
                        printf("Correct! You found a new word.\n");
                    }
                    break;
                }
                gameCurrent = gameCurrent->next;
            }
            if(!found)
            {
                printf("This word is not in the dictionary.\n");
            }
        }
        else
        {
            printf("this is not a word using the characters in the master word!!\n"); //Otherwise display that the user has "lost" or guessed a word that is not using the valid letters
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
 *
 * insertIntoDictionary: takes the root and a new word to insert. adds the word to the end of the linked list
 *
 * insertIntogamelist: takes the root and a new word to insert. adds the word to the end of the linked list
 *
 * getRandomWord: gets a random node from the rand function, then iterates from that node to the end, returning the first 7 character or above word
 *
 * findWords: takes the masterword from the getRandomWord function. iterates through the dictionary, only adding words to the
 * gamelist linked list that can be made using the letters from the masterwords characters
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

void insertIntoDictionary(wordListNode **root, char *new_word) {

    wordListNode *newNode = malloc(sizeof(wordListNode));

    if (newNode == NULL) { //Checking if malloc failed
        printf("Memory allocation failed for insertIntoList\n");
        cleanupGameListNodes(&game_root);
        cleanupGameListNodes(&root);
        exit(1);
    }

    strcpy((*newNode).word, new_word);
    (*newNode).next = NULL;


    if (*root == NULL) {
        *root = newNode;
    } else {

        wordListNode *current = *root;
        while ((*current).next != NULL) {
            current = (*current).next;
        }

        (*current).next = newNode;
    }
}

void insertIntoGamelist(gameListNode **game_root, char *new_word) {


    //checking if word already exists
    gameListNode *current = *game_root;
    while (current != NULL) {
        if (strcmp(current->word, new_word) == 0) {
            return;
        }
        current = current->next;
    }

    gameListNode *newNode = malloc(sizeof(gameListNode));

    if (newNode == NULL) { //Checking if the malloc failed
        printf("Memory allocation failed for insertIntogameList\n");
        cleanupGameListNodes(&game_root);
        cleanupGameListNodes(&root);
        exit(1);
    }

    strncpy((*newNode).word, new_word, 29);
    (*newNode).already_found = false;
    (*newNode).next = NULL;


    if (*game_root == NULL) {
        *game_root = newNode;
    } else {

        gameListNode *current = *game_root;
        while ((*current).next != NULL) {
            current = (*current).next;
        }

        (*current).next = newNode;
    }
}

wordListNode *getRandomWord() {
    if (word_counter == 0) { //Checking if the dictionary has any words
        printf("The dictionary is empty.\n");
        cleanupGameListNodes(&game_root);
        cleanupGameListNodes(&root);
        exit(1);
    }

    int randomNode = rand() % word_counter; //getting the random node

    wordListNode *current = root;
    int count = 0;


    while (current != NULL && count < randomNode) {
        current = (*current).next;
        count++;
    }


    while (current != NULL) {
        if (strlen((*current).word) > 6) {
            return current;
        }
        current = (*current).next;
    }


    printf("Was not able to find a word over 6 characters starting from index %d\n", randomNode);
    exit(1);
}

void findWords(wordListNode *masterword) {
    strcpy(validLetters, (*masterword).word);
    make_upper(validLetters);
    getLetterDistribution(validLetters, validLettersdistro);

    wordListNode *current = root;
    int distro[26];

    while (current != NULL) {
        char tempWord[30];
        strcpy(tempWord, (*current).word);
        make_upper(tempWord);


        getLetterDistribution(tempWord, distro);


        if (compareCounts(distro, validLettersdistro)) {
            insertIntoGamelist(&game_root, tempWord);
        }

        current = (*current).next;
    }



}

void cleanupWordListNodes(wordListNode **root) {
    wordListNode *current = *root;
    wordListNode *nextNode;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }

    *root = NULL;  // Set the root pointer to NULL after cleaning up
}

void cleanupGameListNodes(gameListNode **game_root) {
    gameListNode *current = *game_root;
    gameListNode *nextNode;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }

    *game_root = NULL;  // Set the game_root pointer to NULL after cleaning up
}


