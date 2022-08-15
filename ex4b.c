/*
 *"calculation of a polynomial - using processes"
 * iyar levi
*/

#include <stdio.h>

#include <pthread.h>

#include <stdlib.h>

#include <string.h>

#include <sys/types.h>

#include <sys/ipc.h>

#include <sys/shm.h>

#include <unistd.h>

#include <sys/wait.h>

#define SIZE 500
#define MEMORY_SIZE 3 * sizeof(int)

typedef struct {
    char poly[SIZE];
    int val;
    int result;
}
        polyAndValue;

int numOfX(char[]);
void freeArgv(char ** , int);
int sumMonom(polyAndValue);

int main() {
    char inputArr[SIZE] = "";
    char polynomArr[SIZE];
    char valueArr[SIZE];
    int status;
    pid_t pidSon1;
    pid_t pidSon2;
    pid_t pidSon3;
    int * ptr;
    key_t key;
    int shm_id;
    int totalSum;
    int freeInt;

    key = ftok("/temp", 'x'); // create a key

    if ((shm_id = shmget(key, MEMORY_SIZE, IPC_CREAT | IPC_EXCL | 0600)) == -1) { // create a shared memory
        perror("failed to get memory");
        exit(1);
    }
    if ((ptr = (int * ) shmat(shm_id, NULL, 0)) < 0) { // pointer to the shared memory
        perror("failed to attach memory");
        exit(1);
    }

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
            polyAndValue pol0;
            pol0.result = atoi(polynomArr);
            printf("%d\n", pol0.result);
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
        if (x == numX) { // check if there is free number, if there is one, put it in freeInt, else, put zero
            freeInt = 0;
        } else {
            freeInt = atoi(tokens[x - 1]);
        }
        polyAndValue allPolynoms[numX]; // array of struct polynum AND value
        for (int in = 0; in < numX; in ++) { // fill the struct with the data
            strcpy(allPolynoms[ in ].poly, tokens[ in ]);
            allPolynoms[ in ].val = intValue;
        }
        if (numX == 1) { // if we need only one fork (only one x)
            if ((pidSon1 = fork()) == 0) {
                ptr[0] = sumMonom(allPolynoms[0]);
                exit(0);
            } else {
                wait( & status);
                totalSum = ptr[0] + freeInt;
                printf("total sum is: %d\n", totalSum);
            }
        }
        if (numX == 2) { // if we need two forks
            if ((pidSon1 = fork() == 0)) {
                ptr[0] = sumMonom(allPolynoms[0]);
                exit(0);
            } else if ((pidSon2 = fork()) == 0) {
                ptr[1] = sumMonom(allPolynoms[1]);
                exit(0);
            } else {
                wait( & status);
                wait( & status);
                totalSum = ptr[0] + ptr[1] + freeInt;
                printf("total sum is: %d\n", totalSum);
            }
        }
        if (numX == 3) { // if we need three forks
            if ((pidSon1 = fork() == 0)) {
                ptr[0] = sumMonom(allPolynoms[0]);
                exit(0);
            } else if ((pidSon2 = fork()) == 0) {
                ptr[1] += sumMonom(allPolynoms[1]);
                exit(0);
            } else if ((pidSon3 = fork()) == 0) {
                ptr[2] += sumMonom(allPolynoms[2]);
                exit(0);
            } else {
                wait( & status);
                wait( & status);
                wait( & status);
                totalSum = ptr[0] + ptr[1] + ptr[2] + freeInt;
                printf("total sum is: %d\n", totalSum);
            }
        }
        freeArgv(tokens, numX); // free the dynamic allocated memory
    }
    printf("goodbye\n");
    shmdt(ptr); // remove the pointer to the shared memory
    shmctl(shm_id, IPC_RMID, NULL); // delete the shared memory
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

int sumMonom(polyAndValue toSum) { // helper function that sum the monom she gets
    int sumFunc;
    int power, molInt;
    int m;
    char mol[SIZE];
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
    return sumFunc;
}
