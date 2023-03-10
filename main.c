#include <stdio.h>
#include <stdlib.h>
#include "utils.h"


/*
 * Global Variables
 */
char dictionary[DICT_SIZE][WORD_SIZE+1];

const int TRUE = 1;
const int FALSE = 0;

//int win = 0;
//int lose = 0;

/*
 * Functions
 */

// Print Word
void print_word(char word[6]){
    for(int i = 0; i < 6; i++){
        printf("%c",word[i]);
    }
}


// Enter To Continue
void to_continue(){
    printf("\nPress enter to continue...");
    getchar();
}

// Generate Random Word
void get_ran_word(char ran_word[6]){
    int rand_num = (rand() % 5757 ) + 1; // Get random number, mod to ensure < 5757, +1 to get in range

    for(int i = 0; i < 6; i++){
        ran_word[i] = dictionary[rand_num][i]; // Index dictionary with the random number and assign to ran_word[]
    }
}


// Check For Char In String
int check_in_string(char character, char string[6]){
    int res = 0;

    for (int i = 0; i < 6; i++){ // Loop through the string
        if(character == string[i]){ // If character = currently indexed char in string, return 1
            res = 1;
            return res;
        }
    }
    return res;
}

int check_in_dictionary(char word[6]){
    int char_valid = 0;
    for(int i = 0; i < DICT_SIZE;i++){
        for (int j = 0; j < 6; j++){
            if (word[j] == dictionary[i][j])
                char_valid++;
        }
        if (char_valid == 6)
            return TRUE;
        char_valid = 0;
    }
    return FALSE;
}

// Word Input
void word_input(char user_word[6]){
    char tempString[6]; //  Input assigned here to validate before assigning to pointer
    int length = 0;
    while(TRUE){
        char input = 0;
        printf("%s","\nEnter a string of letters --> ");

        while(input!='\n' && length < 6){
            input = getchar();
            tempString[length] =  input;
            length++;
        }
        tempString[5] = 0;

        // If uppercase, convert to lower
        // Check if within a-z range
        // If not, break
        int valid = 1;
        for (int i = 0; i < 5; i++){
            if(tempString[i] >= 'A' && tempString[i] <= 'Z'){
                tempString[i] = tempString[i] + 32;
            }

            if(!(tempString[i] >= 'a' && tempString[i] <= 'z')){
                valid = 0;
                break;
            }
        }
        // Check if word is in dictionary
        valid = check_in_dictionary(tempString);



        if(valid){
            // Copy tempString[] to user_word[]
            for (int i = 0; i < 6; i++) {
                user_word[i] = tempString[i];
            }
            break;
        }

        else{
            printf("please enter a valid word.");
            // Reset tempString and length to 0 if not valid
            for(int i = 0; i < 6; i++){
                tempString[i] = 0;
            }
            length = 0;
        }
    }
}


// Compare Words
void compare_words(char ran_word[6],char user_word[6],
                   char correctly_placed_letters[5],char correct_letters[5],
                    int *correct_letter_count, int *correctly_placed_letter_count){
    /*
     * If: Check if char is in correct placement of ran_word.
     *      -> if so, check if it is not already in correctly_placed_letters[]
     *      -> if it is not, add it to correctly_placed_letters[]
     *      -> Check if it is in correct_letters
     *      -> if it is not, add it to correct_letters
     *
     * Else If: check if char is present in ran_word
     *      -> if so, check if it is not already in correct_letters[]
     *      -> if it is not, add it to correctly_placed_letters
     */
    for(int i = 0; i < 5; i++){

        if(*correctly_placed_letter_count == 5){
            break;
        }
        if(user_word[i] == ran_word[i]){
            if(correctly_placed_letters[i] == '_')
                *correctly_placed_letter_count += 1;

            correctly_placed_letters[i] = user_word[i];
        }

         if(check_in_string(user_word[i],ran_word) && !check_in_string(user_word[i],correct_letters))
         {
             correct_letters[*correct_letter_count] = user_word[i];
             *correct_letter_count += 1;
         }
    }
}



/*
 * Modes
 */

void normal_mode(){

    // Variables
    char ran_word[6] = {0}; // String to store the random word
    char user_word[6] = {0}; //  String for user inputted word
    char correctly_placed_letters[6] = {0}; // String of correctly guessed letters, in the correct position
    char correct_letters[6] = {0}; // String of correctly guessed letters, that may not be in the correct position

    int lives = 0;
    int correct_letter_count = 0;
    int correctly_placed_letter_count = 0;

    // Game Loop
    int run_game = TRUE;// Run game, then ask user to repeat on game end
    while(run_game) {

        // Initialise variables
        lives = 6;
        get_ran_word( ran_word);
        for(int i = 0; i < 6; i++){
            correctly_placed_letters[i] = '_';
            correct_letters[i] = 0;
        }
        correctly_placed_letters[5] = 0;
        correct_letter_count = 0;
        correctly_placed_letter_count = 0;

        // Loop for life tracking
        while(lives > 0){
            word_input(user_word);
            compare_words(ran_word,user_word,correctly_placed_letters,correct_letters,&correct_letter_count,&correctly_placed_letter_count);

            printf("\n%s","Random word: ");
            print_word(ran_word);

            // If the whole word is correct, break
            if (correctly_placed_letter_count == 5){
//                win++;
                printf("\n%s", "Correct, you win!");
                break;
            }

            // Else if lives == 1, trigger game over. Doing so while lives == 1 as lives is decremented after the win check.
            else if(lives == 1){
//                lose++;
                printf("\n%s", "Game Over! The word was....");
                print_word(ran_word);
                break;
            }

            else{
                lives --;
                printf("\nCorrect letter count:%d",correct_letter_count);
                printf("\nCorrect Placed letter count:%d",correctly_placed_letter_count);
                printf("\n%s", "Correctly guessed: ");
                print_word(correctly_placed_letters);
                printf("\n%s", "Correct letters: ");
                print_word(correct_letters);
                printf("\nYou have %d attempts remaining...",lives);
            }
        }

        break;
    }


}

void hard_mode(){
    printf("%s","\nhard mode");

}

void stats_mode(){
//    int total = win+lose;
    printf("%s","Stats:\n");
//    printf("Wins: %d",win);
//    printf("Loses: %d",lose);
//    printf("Total: %d",total);
//    printf("Win Rate: f %%",win/total);

}


/*
 * Main
 */

int main(){
    load_word_list(dictionary); // Load word_list.txt into dictionary[][]

    char input = 0;
    while (TRUE){
    printf("Word Game!\n-----------\n\t1. Normal Mode\n\t2. Hard More\n\t3. View Stats \n\t4. Quit\n -->");
    input = getchar(); getchar();
    switch (input) {
        case '1':
            normal_mode();
            to_continue();
            break;
        case '2':
            hard_mode();
            to_continue();
            break;
        case '3':
            stats_mode();
            to_continue();
            break;
        case '4':
            exit(0);
        default:
            printf("Invalid Input. Please select 1,2,3, or 4.");
        }
    }
}