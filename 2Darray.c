#include <stdio.h>
#include <stdlib.h>
#include <time.h> // For seeding rand()

void invert(int **array, int rows, int cols) {
    if (array == NULL) {
        return; // Handle null pointer
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (array[i][j] == 1) {
                array[i][j] = 0;
            } else {
                array[i][j] = 1;
            }
            printf("%d", array[i][j]);
        }
        printf("\n");
    }
}

int **initialize(int rows, int cols) {

    // Initialize and allocate memory for the 2D array
    int **array = (int **)malloc(rows * sizeof(int *));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed for rows.\n");
        return NULL; // Indicate error
    }

    for (int i = 0; i < rows; i++) {
        array[i] = (int *)malloc(cols * sizeof(int));
        if (array[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for row %d.\n", i);
            // Free previously allocated rows
            for (int k = 0; k < i; k++) {
                free(array[k]);
            }
            free(array);
            return NULL; // Indicate error
        }
    }

    // Populate array with random 0 or 1
    srand(time(NULL)); // Seed the random number generator
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            array[i][j] = rand() % 2;
            printf("%d", array[i][j]);
        }
        printf("\n");
    }

    return array;

}

int main() {
    int rows = 4;
    int cols = 4;

    int** matrix = initialize(rows, cols);

    invert(matrix, rows, cols);

    // Free allocated memory
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}