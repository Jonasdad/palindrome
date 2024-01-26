#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>

int numberOfWords = 25144;
int countedWords = 0;
char* words[25144];
FILE *input;
FILE *output;
char* reverse(char* word);
int palindrome(char* word);
void printArray(char** array, int length);
int compare(const void* a, const void* b);
void* readFile(char** words);


int main(int argc, char *argv[]){
    input = fopen("words.txt", "r");
    output = fopen("output.txt", "w");
    if(input == NULL){
        printf("Error opening file\n");
        return 1;
    }
    readFile(words);
    
    
    
    for(int i = 0; i < numberOfWords; i++){
        if(palindrome(words[i])){
            printf("%s is a palindrome\n", words[i]);
            fprintf(output, "%s\n", words[i]);
            countedWords++;
        }
        else{
            char* rev = reverse(words[i]);
            //printf("%s is not a palindrome. Checking %s\n", words[i], rev);
            if(bsearch(&rev, words, numberOfWords, sizeof(char*), compare) == NULL){
                continue;
            }
            else{
                printf("%s is palindromic\n", rev);
                fprintf(output, "%s\n", words[i]);
                countedWords++;
            }
        }
    }
    printf("%d\n", countedWords);

}
#include <string.h>
#include <ctype.h>

char* removeApostrophes(const char* str) {
    char* newStr = malloc(strlen(str) + 1);
    int i = 0;
    while (*str) {
        if (*str != '\'') {
            newStr[i++] = *str;
        }
        str++;
    }
    newStr[i] = '\0';
    return newStr;
}

int compare(const void* a, const void* b){
    char* str1 = removeApostrophes(*(const char**)a);
    char* str2 = removeApostrophes(*(const char**)b);
    int result = strcasecmp(str1, str2);
    free(str1);
    free(str2);
    return result;
}
char* reverse(char* word){
    int length = strlen(word);
    if (word[length - 1] == '\n') {
        length--; // Ignore the newline character
    }
    char* reversed = malloc(sizeof(char) * (length + 1)); // Allocate space for null character
    for(int i = 0; i < length; i++){
        reversed[i] = word[length - i - 1];
    }
    reversed[length] = '\0'; // Null-terminate the reversed string
    return reversed;
}


void* readFile(char** words){
    char* line = malloc(sizeof(char) * 64);
    int i = 0;

    while(fgets(line, 64, input)){
        int len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0'; // Replace newline with null character
        }
        words[i] = malloc(sizeof(char) * 64);
        strcpy(words[i], line);
        i++;
    }
    free(line);
    fclose(input);
}


int palindrome(char* word){
    char* reversed = reverse(word);
    if(strcasecmp(word, reversed) == 0){
        return 1;
    }
    free(reversed);
    return 0;
}

void printArray(char** array, int length){
    for(int i = 0; i < length; i++){
        printf("%s", array[i]);
    }
}

