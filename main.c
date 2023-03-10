#include <stdio.h>
#include <stdlib.h>
#include "utils.h"


/*
 * Global Variables
 */
char dictionary[DICT_SIZE][WORD_SIZE+1];

const int TRUE = 1;
const int FALSE = 0;

int win = 0;
int lose = 0;

char alphabet[26] = {0};
int win_guess_archive[100] = {}; // counts how many guesses it took for each win
int win_alphabet_use_archive[100] = {}; // counts how many letters of the alphabet were used

int game_no = -1;   // Tracks which game the user is currently on. -1 as it will be incremented to 0 at the start of
                    // the game. Used to index win_guess_archive


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
        if (char_valid == 5)
            return TRUE;
        char_valid = 0;
    }
    return FALSE;
}

// Word Input
void word_input(char user_word[6], char difficulty,char correctly_placed_letters[5]
                ,char partially_correct_letters[5],int partially_correct_letters_count,
                char incorrect_letters[26], int incorrect_letters_count, int alphabet_use_chars[]){
    char tempString[6]; //  Input assigned here to validate before assigning to pointer
    int length = 0;
    while(TRUE){
        fflush(stdin); // Clear the input buffer
        char input = 0;
        printf("%s","\nEnter a string of letters --> ");

        while(input!='\n' && length < 6){
            input = getchar();
            tempString[length] =  input;
            length++;
        }
        tempString[6] = 0;

        // If uppercase, convert to lower
        // Check if within a-z range
        // If not, break

        int valid = 1;
        valid = check_in_dictionary(tempString); // Check if in dictionary


        // For hard mode
        int correct_and_checked_count = 0;  // This will keep track of which correct characters have been checked,
        char correct_and_checked[6] = {0};  // So the program doesn't consider words with repeated chars (i.e. spots)
                                            // to be invalid.

        for (int i = 0; i < 5; i++){

            if(tempString[i] >= 'A' && tempString[i] <= 'Z'){
                tempString[i] = tempString[i] + 32;
            }

            if(!(tempString[i] >= 'a' && tempString[i] <= 'z')){
                valid = 0;
                break;
            }



            // Check if the difficulty mode is hard, do the required checks
            if(difficulty == 'h'){
                // Check if character is in the correctly_placed letter string & if it is in the correct placement there
                if(check_in_string(tempString[i],correctly_placed_letters)
                                            && tempString[i] != correctly_placed_letters[i]){

                    // Check if the character has already been checked. If it hasn't, valid = 0 and break.
                    if(!check_in_string(tempString[i],correct_and_checked)){
                        valid = 0;
                        break;
                    }

                }
                // if it is present in the right location, add to the list array of checked characters
                else if (check_in_string(tempString[i],correctly_placed_letters)
                         && tempString[i] == correctly_placed_letters[i]){
                        correct_and_checked[correct_and_checked_count] = tempString[i];
                        correct_and_checked_count++;
                }

                // Check if the characters in partially_correct_letters[] are present in tempString
                //  If not, valid = 0 and break
                if( partially_correct_letters_count > 0){
                    for(int i = 0; i < partially_correct_letters_count;i++){
                        if(!check_in_string(partially_correct_letters[i],tempString) ){
                            valid = 0;
                            break;
                        }
                    }
                }

                // Check if the characters in incorrect_letters[] are present,
                // If so, valid = 0 and break
                if( incorrect_letters > 0){
                    for(int i = 0; i < incorrect_letters_count;i++){
                        if(check_in_string(incorrect_letters[i],tempString) ){
                            valid = 0;
                            break;
                        }
                    }
                }

            }
        }



        if(valid){
            // Copy tempString[] to user_word[]
            for (int i = 0; i < 6; i++) {
                user_word[i] = tempString[i];

                // Check if in alphabet
                for(int j = 0; j < 26;j++){
                    if(tempString[i] == alphabet[j]){
                        alphabet_use_chars[j] += 1;
                    }
                }

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
                   char correctly_placed_letters[5],char partially_correct_letters[5],
                    int *partially_correct_letter_count, int *correctly_placed_letter_count,
                    char difficulty, char incorrect_letters[26], int *incorrect_letters_count){
    /*
     * If: Check if char is in correct placement of ran_word.
     *      -> if so, check if it is not already in correctly_placed_letters[]
     *      -> if it is not, add it to correctly_placed_letters[]
     *      -> Check if it is in partially_correct_letters
     *      -> if it is not, add it to partially_correct_letters
     *
     * Else If: check if char is present in ran_word
     *      -> if so, check if it is not already in partially_correct_letters[]
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

         if(check_in_string(user_word[i],ran_word) &&
            !check_in_string(user_word[i],partially_correct_letters)){

             partially_correct_letters[*partially_correct_letter_count] = user_word[i];
             *partially_correct_letter_count += 1;
         }
            // If hard mode, add to list of incorrect letters if not present
         else if (difficulty == 'h' &&!check_in_string(user_word[i],partially_correct_letters)){
             for (int j = 0; j < 26; j++){
                 if (user_word[i] != incorrect_letters[j]){
                     incorrect_letters[*incorrect_letters_count] = user_word[i];
                     *incorrect_letters_count += 1;
                     break;
                 }
             }
         }
    }
}



/*
 * Modes
 */

void game_mode(char difficulty){


    // Variables
    char ran_word[6] = {0};
    char user_word[6] = {0}; //  String for user inputted word
    char correctly_placed_letters[6] = {0}; // String of correctly guessed letters, in the correct position
    char partially_correct_letters[6] = {0};    // String of correctly guessed letters,
                                                // that may not be in the correct position

    int lives = 6;
    int guesses = 0;
    int partially_correct_letter_count = 0;
    int correctly_placed_letter_count = 0;
    int alphabet_use_chars[26] = {0};   // each index of the array counts how many of the corresponding char
                                            // in alphabet were used in each game. This is done so we
                                            // don't simply iterate each time a character is used,
                                            //  and get an incorrect percentage in stats

    // Hard Mode specific
    char incorrect_letters[26] = {0}; // String that collects incorrect letters inputted
    int incorrect_letters_count = 0;


    game_no++; // Increment the game no.
    // Initialise variables
    get_ran_word( ran_word);
    for(int i = 0; i < 6; i++){
        correctly_placed_letters[i] = '_';
        partially_correct_letters[i] = 0;
    }
    for(int i = 0; i < 26; i++){
        incorrect_letters[i] = 0;
        alphabet_use_chars[i] = 0;
    }



    // Loop for life tracking
    while(lives > 0){
        word_input(user_word,difficulty,correctly_placed_letters,partially_correct_letters,
                   partially_correct_letter_count,incorrect_letters,incorrect_letters_count,alphabet_use_chars);

        guesses++; // Increment number of guesses after guess is made

        compare_words(ran_word,user_word,correctly_placed_letters,partially_correct_letters,
                      &partially_correct_letter_count,&correctly_placed_letter_count,difficulty,
                      incorrect_letters,&incorrect_letters_count);

        printf("\n%s","Random word: ");
        print_word(ran_word);

        // If the whole word is correct, break
        if (correctly_placed_letter_count == 5){
            win++;
            printf("\n%s", "Correct, you win!");
            win_guess_archive[game_no] = guesses;

            // Calculate characters of alphabet used and record accordingly
            int char_use_count = 0;
            for(int i = 0; i < 26; i++){
                if(alphabet_use_chars[i] >= 1){
                    char_use_count++;
                }
            }
            win_alphabet_use_archive[game_no] = char_use_count;
            break;
        }

        // Else if lives == 1, trigger game over. Doing so while lives == 1 as lives is decremented after the win check.
        else if(lives == 1){
            lose++;
            printf("\n%s", "Game Over! The word was....");
            print_word(ran_word);
            win_guess_archive[game_no] = guesses;

            // Calculate characters of alphabet used and record accordingly
            int char_use_count = 0;
            for(int i = 0; i < 26; i++){
                if(alphabet_use_chars[i] >= 1){
                    char_use_count++;
                }
            }
            win_alphabet_use_archive[game_no] = char_use_count;
            break;
        }

        else{
            lives --;
            printf("\n%s", "Correctly guessed: ");
            print_word(correctly_placed_letters);
            printf("\n%s", "Correct letters: ");
            print_word(partially_correct_letters);
            if(difficulty == 'h'){ // If hard mode print incorrect letters
                printf("\nIncorrect Letters: ");
                for(int i = 0; i < 26;i++){
                    printf("%c",incorrect_letters[i])   ;
                }
            }


            printf("\nYou have %d attempts remaining...",lives);
        }
    }


}


void stats_mode(){
    float win_rate = ((float)win/ (float)(win+lose))*100;
    printf("\nStats:\n");
    printf("\nWins: %d",win);
    printf("\nLoses: %d",lose);
    printf("\nTotal: %d",game_no);
    printf("\nWin Rate: %f%%",win_rate);

    printf("\nGuesses per game: ");
    for (int i = 0; i <= game_no; i++){
        printf("\n\t%d: ",i+1);
        for(int j = 0; j < win_guess_archive[i];j++)
            printf("%c",'*');

    }
    printf("\n %% of alphabet used per game:");
    for (int i = 0; i <= game_no; i++){
        printf("\n\t%d: %f%%",i+1, (float) win_alphabet_use_archive[i] / 26.0);
    }

}


/*
 * Main
 */

int main(){
    load_word_list(dictionary); // Load word_list.txt into dictionary[][]

    for(int i = 0; i < 26; i++){
        alphabet[i] = i+'a'; // Loop through ascii values to create alphabet array
    }

    char input = 0;
    while (TRUE){
    printf("Word Game!\n-----------\n\t1. Normal Mode\n\t2. Hard Mode\n\t3. View Stats \n\t4. Quit\n -->");
    input = getchar(); getchar();
    switch (input) {
        case '1':
            game_mode('n');
            to_continue();
            break;
        case '2':
            game_mode('h');
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