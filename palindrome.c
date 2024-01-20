#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256

int isPalindrome(char *s);
char* binarySearch(FILE* file, char* key);
char* reverse(char* s);
void stringToLower(char *s);
//void* work(void *arg);

FILE *output;
FILE *input;
int palindromic = 0;

int main(int argc, char *argv[]){
    int palindromic = 0;
    int palindromeWord = 0;
    FILE *input = fopen("words.txt", "r");
    FILE *output = fopen("output.txt", "w");
    if(input == NULL){
        printf("Error opening file\n");
        return 1;
    }
    char line[256];
    
    while(fgets(line, sizeof(line), input)){
        line[strlen(line) - 1] = '\0';
        char* test = "mimicking"; 
        if(binarySearch(input, test)){
            printf("%s found\n", line);
            break;
         }
         else{
            printf("%s not found\n", test);
            break;
         }
    }
    
    
    fclose(input);
    return 0;
}
/*
void* work(void *arg){
    line[strlen(line) - 1] = '\0';
    stringToLower(line);
    if(isPalindrome(line)){
        printf("%s is a palindrome\n", line);
        fprintf(output, "%s\n", line);
        palindromic++;
    }
    char* reversed = reverse(line);
    char* result = binarySearch(input, reversed);
    if(result != NULL){
        printf("%s is a palindromic word\n", result);
        palindromic++;
    }
    free(reversed);
    free(result);
    return NULL;
}
*/

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
    long left = 0;
    fseek(file, 0, SEEK_END);
    long right = ftell(file);
    while(left <= right){
        long mid = left + (right - left ) / 2;
        fseek(file, mid, SEEK_SET);
        if (mid != 0) { // if not at the start of the file
            int c;
            while ((c = fgetc(file)) != '\n' && c != EOF); // discard until newline
    }
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