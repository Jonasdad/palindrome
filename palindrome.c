#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include "queue.c"

//#define MAX_WORDS 25143
#define MAX_WORDS 104335
#define PATH "words2.txt"
int countedWords = 0;
const char *words[MAX_WORDS];
const char *words2[MAX_WORDS];
queue *bagoftasks;

FILE *input;
FILE *output;
FILE *logg;

char *reverse(char *word);
int palindrome(char *word);
int compare404(const void *a, const void *b);
int compare426(const void *a, const void *b);
void *readFile(queue *taskQueue);
void *work();

pthread_mutex_t mutex;

int main(int argc, char *argv[])
{
    if((argv[1] == NULL) || (argc > 2)){
        printf("Usage: No argument given\n");
        return 1;
    }
    bagoftasks = create_queue();
    input = fopen(PATH, "r");
    output = fopen("output.txt", "w");
    logg = fopen("log.txt", "w");
    if (input == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }
    readFile(bagoftasks);
    fclose(input);
    qsort(words, MAX_WORDS, sizeof(char *), compare404);

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
    fclose(output);
    return 0;
}
void *work()
{
    int localCount = 0;
    while (1)
    {
        char *word = dequeue(bagoftasks);
        if (word == NULL)
        {
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
            if (bsearch(&rev, words, MAX_WORDS, sizeof(char*), compare404) != NULL)
            {
                pthread_mutex_lock(&mutex);
                fprintf(output, "%s\n", word);
                localCount++;
                pthread_mutex_unlock(&mutex); // Unlock the mutex before continue
                continue;
            }
        }
        free(word); // Remember to free the word after processing
    }
    int *result = malloc(sizeof(int));
    *result = localCount;
    return result;
}
void remove_special_characters(char *str) {
    int i = 0, j = 0;
    while (str[i]) {
        if (isalnum(str[i]) || str[i] == ' ') {
            str[j] = str[i];
            j++;
        }
        i++;
    }
    str[j] = '\0';
}


int compare404(const void*a, const void *b){
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcasecmp(*ia, *ib);
}

int compare426(const void *a, const void *b)
{
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;

    char *copy_a = strdup(*ia);
    char *copy_b = strdup(*ib);

    remove_special_characters(copy_a);
    remove_special_characters(copy_b);

    int result = strcasecmp(copy_a, copy_b);

    free(copy_a);
    free(copy_b);

    return result;
}
char *reverse(char *word)
{
    int length = strlen(word);
    char *reversed = strdup(word); // Allocate space for null character
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
        char *word = strdup(line);
        enqueue(taskQueue, word);
        words[index] = word; // Add word to words array
        words2[index] = word;
        index++;
    }
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