#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


int binarySearch(char words[][64], int low, int high, char* key);
int numberOfWords = 25144;
int countedWords = 0;

int main(int argc, char *argv[]){
    printf("Running\n");
    //  Open input file
    FILE *input = fopen("words.txt", "r");
    //  Open search file for binary search to 
    //  avoid losing file pointer while searching
    FILE *searchFile = fopen("words.txt", "r");
    if(input == NULL){
        printf("Error opening file\n");
        return 1;
    }

    //  2D array of characters to store words of 63 characters or less
    char words[25144][64];
    char* line = malloc(sizeof(char) * 64);
    int i = 0;
    //  Read words from file into array
    while(fgets(line, 64, input)){
        char* token = strtok(line, " \n");
        while(token != NULL){
            // Convert word to lowercase
            for(int j = 0; token[j]; j++){
                token[j] = tolower((unsigned char)token[j]);
            }
            // Copy word to array
            strcpy(words[i], token);
            i++;
            token = strtok(NULL, " \n");
        }
    }
    free(line);
    printf("%d words loaded\n", i);
    printf("First word: %s\n", words[0]);
    printf("Last word: %s\n", words[25143]);

    for(int j = 0; j < numberOfWords; j++){
    if(binarySearch(words, 0, numberOfWords, words[j]) == -1){
        printf("Word not found: %s\n", words[j]);
    }
    else {
        countedWords++;
    }
}
    printf("%d words found\n", countedWords);

    return 0;
}

int binarySearch(char words[][64], int low, int high, char* key){
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int res = strcmp(words[mid], key);

        // Check if key is present at mid
        if (res == 0)
            return mid;

        // If key greater, ignore left half
        if (res < 0)
            low = mid + 1;

        // If key is smaller, ignore right half
        else
            high = mid - 1;
    }

    return -1; // if we reach here, then element was not present
}