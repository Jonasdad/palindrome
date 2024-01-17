#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256

int isPalindrome(char *s);
char* binarySearch(FILE* file, char* key);
char* reverse(char* s);
void stringToLower(char *s);

int main(int argc, char *argv[]){
    int numberOfPalindromes = 0;
    int palindromeWord = 0;
    FILE *file = fopen("words.txt", "r");
    if(file == NULL){
        printf("Error opening file\n");
        return 1;
    }

    char line[256];
    while(fgets(line, sizeof(line), file)){
        line[strlen(line) - 1] = '\0';
        stringToLower(line);
        if(isPalindrome(line)){
            printf("%s is a palindrome\n ", line);
            numberOfPalindromes++;
        }
        char* reversed = reverse(line);
        char* result = binarySearch(file, reversed);
        if(result != NULL){
            printf("%s is a palindromic word\n ", result);
            palindromeWord++;
        }
        free(reversed);
        free(result);
    }
    printf("Number of palindromes: %d ", numberOfPalindromes);
    printf("Number of palindromic words: %d ", palindromeWord);
    fclose(file);
    return 0;
}
void stringToLower(char *s){
    for(int i = 0; s[i]; i++){
        s[i] = tolower(s[i]);
    }
}

char* reverse(char* s){
    char* result = malloc(strlen(s) + 1);
    int i = 0;
    int j = strlen(s) - 1;
    while(i < strlen(s)){
        result[i] = s[j];
        i++;
        j--;
    }
    result[i] = '\0';
    return result;

}


char* binarySearch(FILE* file, char* key){
    long originalPosition = ftell(file);
    fseek(file, 0, SEEK_END);
    long left = 0;
    long right = ftell(file) / MAX_LINE_LENGTH;
    while(left <= right){
        long mid = left + (right - left ) / 2;
        fseek(file, mid * MAX_LINE_LENGTH, SEEK_SET);
        char line[MAX_LINE_LENGTH];
        fgets(line, sizeof(line), file);
        line[strlen(line) - 1] = '\0';
        int comparison = strcmp(line, key);
        if(comparison == 0){
            char* result = malloc(strlen(line) + 1);
            strcpy(result, line);
            return result;
        }
        else if(comparison < 0){
            left = mid + 1;
        }
        else{
            right = mid - 1;
        }
    }
    fseek(file, originalPosition, SEEK_SET);
    return NULL;
}

int isPalindrome(char *s){
    int i = 0;
    int j = strlen(s) - 1;
    while(i < j){
        if(s[i] != s[j]){
            return 0;
        }
        i++;
        j--;
    }
    return 1;
}