#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include "queue.c"

int numberOfWords = 26000;
int countedWords = 0;
int palindromes = 0;
int palindromic = 0;
const char *words[30000];
queue *bagoftasks;

FILE *input;
FILE *output;

char *reverse(char *word);
int palindrome(char *word);
void printQueue(queue *q);
int compare(const void *a, const void *b);
void *readFile(queue *taskQueue);
char *removeApostrophes(const char *str);
void *work(void *counter);

pthread_mutex_t mutex;

int main(int argc, char *argv[])
{
    bagoftasks = create_queue();
    input = fopen("words.txt", "r");
    output = fopen("output.txt", "w");
    if (input == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }
    readFile(bagoftasks);
    fclose(input);
 //   printQueue(bagoftasks);
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
            pthread_mutex_lock(&mutex);
            if (bsearch(&rev, words, numberOfWords, sizeof(char *), compare) == NULL)
            {
                pthread_mutex_unlock(&mutex); // Unlock the mutex before continue
                continue;
            }
            else
            {
                fprintf(output, "%s\n", word);
                localCount++;
            }
            pthread_mutex_unlock(&mutex);
        }
        free(word); // Remember to free the word after processing
    }
    int *result = malloc(sizeof(int));
    *result = localCount;
    return result;
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

int compare(const void *a, const void *b)
{
    char *str1 = removeApostrophes(*(const char **)a);
    char *str2 = removeApostrophes(*(const char **)b);
    int result = strcasecmp(str1, str2);
    free(str1);
    free(str2);
    return result;
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
