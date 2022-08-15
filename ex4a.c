/*
 * "calculation of a polynomial - using threads"
 * iyar levi
*/

#include <stdio.h>

#include <pthread.h>

#include <stdlib.h>

#include <string.h>

#define SIZE 500

int numOfX(char[]);
void freeArgv(char ** , int);
void * sumMonom(void * arg);

typedef struct {
    char poly[SIZE];
    int val;
}
        polyAndValue;

int main() {
    char inputArr[SIZE] = "";
    char polynomArr[SIZE];
    char valueArr[SIZE];
    int sumResult = 0;
    int status;
    int y;
    int * res;

    while (strcmp(inputArr, "done") != 0) {
        printf("please enter polynom and a value:\n");
        fgets(inputArr, SIZE, stdin);
        inputArr[strlen(inputArr) - 1] = '\0';
        if (strcmp(inputArr, "done") == 0) { // if the input is "done" - finish the program
            break;
        }
        int index;
        for (index = 0; inputArr[index] != ','; ++index) { // put the polynom in the polynomArr
            polynomArr[index] = inputArr[index];
        }
        polynomArr[index] = '\0';

        int j;
        int i = 0;
        for (j = index + 2; inputArr[j] != '\0'; ++j) { // put the value in the valueArr
            valueArr[i] = inputArr[j];
            i++;
        }
        valueArr[i] = '\0';
        i = 0;
        int intValue = atoi(valueArr); // use the input value as int
        int numX = numOfX(polynomArr); // the int value of the input

        if (numX == 0 && strlen(polynomArr) != 0) { // if there is no x in the polynom
            sumResult = atoi(polynomArr);
            printf("sum is: %d\n", sumResult);
            sumResult = 0;
            continue;
        }
        int x = 0;
        char ** tokens = (char ** ) malloc(sizeof(char * ) * (numX + 1)); // dynamic allocated memory
        char * piece = strtok(polynomArr, "+");
        while (piece != NULL) { // fill the tokens array with the pieces of the polynom
            tokens[x] = (char * ) malloc(sizeof(char) * (strlen(piece))); // dynamic allocated memory
            strcpy(tokens[x], piece);
            x++;
            piece = strtok(NULL, "+"); // splite the polynom by the "+" delimiter
        }
        polyAndValue allPolynoms[numX]; // array of struct polynum AND value
        for (int in = 0; in < numX; in ++) { // fill the struct with the data
            strcpy(allPolynoms[ in ].poly, tokens[ in ]); // update the struct value
            allPolynoms[ in ].val = intValue; // update the struct value
        }

        pthread_t threadsArray[numX];
        for (int k = 0; k < numX; ++k) { // creating theareds and run them
            status = pthread_create( & threadsArray[k], NULL, sumMonom, (void * ) & allPolynoms[k]);
            if (status != 0) { // check if there is no problems
                fputs("pthread create failed\n", stderr);
                exit(EXIT_FAILURE);
            }
        }

        for (int k = 0; k < numX; ++k) { // "free" the threads
            pthread_join(threadsArray[k], (void ** ) & res);
            sumResult += * res;
        }
        if (x == numX) { // if there is no free number
            printf("sum is: %d\n", sumResult);
        } else { // if there is free number
            y = atoi(tokens[x - 1]);
            sumResult += y;
            printf("sum is: %d\n", sumResult);
        }
        y = 0;
        sumResult = 0;
        freeArgv(tokens, numX); // free the dynamic allocated memory
    }
    printf("goodbye\n");
    return 0;
}

int numOfX(char pol[]) { // helper function that count how many x is in the polynom
    int howManyX = 0;
    for (int i = 0; pol[i] != '\0'; ++i) {
        if (pol[i] == 'x') {
            howManyX++;
        }
    }
    return howManyX;
}

void freeArgv(char ** Arr, int cells) { // helper function that free al the dynamic allocated memory
    for (int ma = 0; ma <= cells; ma++) { // free all the dynamic allocated memory
        free(Arr[ma]);
    }
    free(Arr);
}

void * sumMonom(void * arg) { // helper function that sum the monom she gets
    int sumFunc;
    int power, molInt;
    int m;
    char mol[SIZE];
    polyAndValue toSum = * ((polyAndValue * ) arg);
    int val = toSum.val;
    char * monom = toSum.poly;
    if (monom[0] == 'x') { // if there is no multiplier
        molInt = 1;
    } else {
        for (m = 0; monom[m] != '*'; ++m) { // the multiplier
            mol[m] = monom[m];
        }
        mol[m] = '\0';
        molInt = atoi(mol); // the multiplier as int
    }
    for (int j = 0; monom[j] != '\0'; ++j) { // catch the power from the monom
        if (monom[j] == '^') {
            power = monom[j + 1] - '0';
        }
    }
    int valAfter = val;
    if (power == 0) { // if the power is zero
        val = 1;
    } else { // if the power is not equal to zero
        for (int jj = 0; jj < power - 1; ++jj) {
            valAfter *= val;
        }
    }
    sumFunc = valAfter * molInt; // the total sum
    int * n = (int * ) malloc(sizeof(int));
    * n = sumFunc;
    return n;
}
