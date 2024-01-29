#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include "queue.c"

int numberOfWords = 25143;
int countedWords = 0;
int palindromes = 0;
int palindromic = 0;
const char *words[25143];
queue *bagoftasks;

FILE *input;
FILE *output;
FILE *logfile;
char *reverse(char *word);
int palindrome(char *word);
void printQueue(queue *q);
int compare_function(const void *a, const void *b);
void *readFile(queue *taskQueue);
char *removeApostrophes(const char *str);
void *work(void *counter);
int linearSearch(char *key);

pthread_mutex_t mutex;

int main(int argc, char *argv[])
{
    if(argv[1] == NULL){
        printf("Usage: No argument given\n");
        return 1;
    }
    bagoftasks = create_queue();
    input = fopen("words.txt", "r");
    output = fopen("output.txt", "w");
    logfile = fopen("log.txt", "w");

    if (input == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }
    readFile(bagoftasks);
    fclose(input);
    //printQueue(bagoftasks);
    pthread_mutex_init(&mutex, NULL);
    int numberOfThreads = atoi(argv[1]);
    pthread_t handler[numberOfThreads];
    int *results[numberOfThreads];

    for (int i = 0; i < numberOfThreads; i++)
    {
        int ret = pthread_create(&handler[i], NULL, &work, NULL);
        if (ret != 0)
        {
            printf("Error: pthread_create failed\n");
            return 1;
        }
        pthread_t tid = pthread_self();
    }

    for(int i = 0; i < numberOfThreads; i++){
        pthread_join(handler[i], (void**)&results[i]);
    }

    for (int i = 0; i < numberOfThreads; i++)
    {
        printf("Thread %d found %d palindromic words\n", i, *results[i]);
        countedWords += *results[i];
        free(results[i]); // Remember to free the result after printing
    }

    printf("Counted palindromic words: %d\n", countedWords);
    printf("Number of words read: %d\n", numberOfWords);
    fclose(output);
    return 0;
}

void *work(void *arg)
{
    int localCount = 0;
    while (1)
    {
        char *word = dequeue(bagoftasks);
        if (word == NULL)
        {
            printf("Debug: No more tasks\n");
            break; // No more tasks
        }
        if (palindrome(word))
        {
            pthread_mutex_lock(&mutex);
            fprintf(output, "%s\n", word);
            localCount++;
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            char *rev = reverse(word);
           // printf("Debug: Reversed word: %s\n", rev);
            pthread_mutex_lock(&mutex);
            //linearSearch(rev) != -1
            //bsearch(&rev, words, numberOfWords, sizeof(char *), compare_function) != NULL
            if (linearSearch(rev) != -1)
            {
                fprintf(logfile, "Debug: %s is a semordnilap\n", word);
                localCount++;
                pthread_mutex_unlock(&mutex); // Unlock the mutex before continue
                continue;
            }
            pthread_mutex_unlock(&mutex);
        }
        free(word); // Remember to free the word after processing
    }
    printf("Debug: Thread finished, found %d semordnilaps\n", localCount);
    int *result = malloc(sizeof(int));
    *result = localCount;
    return result;
}
int linearSearch(char *key) {
    for (int i = 0; i < numberOfWords; i++) {
        if (strcasecmp(words[i], key) == 0) {
            printf("Debug: Found %s at index %d\n", key, i);
            return i; // Return the index of the found word
        }
    }
    return -1; // Return -1 if the word was not found
}

char *removeApostrophes(const char *str)
{
    char *newStr = malloc(strlen(str) + 1);
    int i = 0;
    while (*str)
    {
        if (*str != '\'')
        {
            newStr[i++] = *str;
        }
        str++;
    }
    newStr[i] = '\0';
    return newStr;
}

int compare_function(const void *a, const void *b) {
    const char *str1 = *(const char **)a;
    const char *str2 = *(const char **)b;

    while (*str1 && *str2) { // while the two strings are not empty
        if (*str1 == '\'') { // if the character is an apostrophe
            str1++; // skip it
            continue;
        }
        if (*str2 == '\'') {
            str2++;
            continue;
        }

        if (tolower((unsigned char)*str1) != tolower((unsigned char)*str2)) { // if the two characters are not equal
            return tolower((unsigned char)*str1) - tolower((unsigned char)*str2); // return the difference
        }

        str1++; // increment the pointers
        str2++; // increment the pointers
    }

    while (*str1 == '\'') str1++; // skip the apostrophes
    while (*str2 == '\'') str2++; // skip the apostrophes

    return *str1 - *str2; // return the difference
}

char *reverse(char *word)
{
    int length = strlen(word);
    char *reversed = malloc(sizeof(char) * (length + 1)); // Allocate space for null character
    for (int i = 0; i < length; i++)
    {
        reversed[i] = word[length - i - 1];
    }
    reversed[length] = '\0'; // Null-terminate the reversed string
    return reversed;
}

void *readFile(queue *taskQueue)
{
    char *line = malloc(sizeof(char) * 64);
    int index = 0;

    while (fgets(line, 64, input))
    {
        int len = strlen(line);
        if (line[len - 1] == '\n')
        {
            line[len - 1] = '\0'; // Replace newline with null character
        }
        for(int i = 0; line[i]; i++){
            line[i] = tolower(line[i]); // Convert to lowercase
        }
        char *word = malloc(sizeof(char) * 64);
        strcpy(word, line);
        enqueue(taskQueue, word);
        words[index] = word; // Add word to words array
        index++;
    }
    numberOfWords = index; // Set numberOfWords to the number of words read
    free(line);
    fclose(input);
}

int palindrome(char *word)
{
    char *reversed = reverse(word);
    if (strcasecmp(word, reversed) == 0)
    {
        return 1;
    }
    free(reversed);
    return 0;
}

void printQueue(queue *q)
{
    int size = 0;
    node *temp = q->head;
    while (temp != NULL)
    {
        printf("%s\n", (char *)temp->data);
        temp = temp->next;
        size++;
    }
    printf("Queue size: %d\n", size);
}
