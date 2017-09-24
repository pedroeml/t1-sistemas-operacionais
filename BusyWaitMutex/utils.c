#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

bool is_even(int number) {
    return number % 2 == 0;
}

bool is_prime(int number) {
    if (number == 1 || number == 2)
        return true;
    else if (is_even(number))
        return false;
    /*
     * Considering it's not an even number, try every possible
     * odd divisor within its half.
     */
    for (int divisor = 3; divisor <= number/2; divisor += 2) {
        if (number % divisor == 0)
            return false;
    }

    return true;    // If this number has no divisor in that range, then it's prime.
}

int** create_ranges(int interval, int length) {
    int** ranges;
    ranges = (int**) malloc(sizeof(int*)*length);

    for (int i = 0; i < length; i++) {
        int* row;
        row = (int*) malloc(sizeof(int)*2);

        *row = 1 + interval*i;
        *(row+1) = interval + interval*i;

        *(ranges+i) = row;
    }

    return ranges;
}

void print_arr(int** arr, int length) {
    printf("[ ");

    int* row;
    for (int i = 0; i < length - 1; i++) {
        row = *(arr + i);
        printf(" [%d, %d]", *row, *(row + 1));
    }

    row = *(arr + length - 1);
    printf(" [%d, %d] ]\n", *row, *(row + 1));
}

int* generate_params(int start, int end, int thread_number) {
    int* params;
    params = (int*) malloc(sizeof(int)*3);

    *params = start;
    *(params+1) = end;
    *(params+2) = thread_number;

    return params;
}
