#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

void printArray(char array[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c", array[i][j]);
        }
        printf("\n");
    }
}

int main() {
    char array[SIZE][SIZE];
    int x = 0, y = 0, step = 0;
    char input, currentChar = 'A';

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            array[i][j] = '.';
        }
    }

    array[y][x] = currentChar;
    
    printArray(array);

    while (1) {
        printf("walk to: \n");
        scanf(" %c", &input);

        if (input == 'x') {
            break;
        }

        int prevX = x, prevY = y;

        switch (input) {
            case 'n': y--; break;
            case 'e': x++; break;
            case 's': y++; break;
            case 'w': x--; break;
        }

        if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || array[y][x] != '.') {
            printf("not possible\n");
            x = prevX;
            y = prevY;
            continue;
        } else {
            currentChar = 'A' + ((step++ + 1) % 26);
            array[y][x] = currentChar;
        }
        printArray(array);
    }

    return 0;
}
