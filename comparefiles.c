#include <stdio.h>
#include <string.h>

int main() {
    FILE *file1 = fopen("outputJonas.txt", "r");
    FILE *file2 = fopen("outputDeniel.txt", "r");

    if (file1 == NULL || file2 == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    char word1[64], word2[64];

    while (fscanf(file1, "%63s", word1) == 1 && fscanf(file2, "%63s", word2) == 1) {
        if (strcmp(word1, word2) != 0) {
            printf("Words differ: %s, %s\n", word1, word2);
        }
    }

    fclose(file1);
    fclose(file2);

    return 0;
}