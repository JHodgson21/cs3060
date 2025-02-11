#include <stdio.h>

int main() {
    int numrows = 3;  // Number of rows in the grid
    int numcols = 4;  // Number of columns in the grid
    int pixels = 0;   // Counter to keep track of the total number of cells

    // Nested for loop to iterate through each cell in the grid
    for (int i = 0; i < numrows; i++) {
        for (int j = 0; j < numcols; j++) {
            pixels++;  // Increment the pixel counter for each cell
        }
    }

    // Print the total number of pixels
    printf("Total pixels: %d\n", pixels);

    return 0;
}
