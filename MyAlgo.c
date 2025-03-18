#include <stdio.h>
#include <stdlib.h>

int main()
{
    int r = 3, c = 3, i, j, count;

    //malloc array r
    int** arr = (int**)malloc(r * sizeof(int*));

    //malloc array r(0)... r(4)
    for (i = 0; i < r; i++){
        arr[i] = (int*)malloc(c * sizeof(int));
    }

    // Assign values to 2D array
    count = 0;
    for (i = 0; i < r; i++)
        for (j = 0; j < c; j++)
            arr[i][j] = ++count;

    // print all array values
    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }

    /* Code for further processing and free the
       dynamically allocated memory */

    for (int i = 0; i < r; i++)
        free(arr[i]);

    free(arr);

    return 0;
}
