#ifndef BUSYWAITMUTEX_UTILS_H
#define BUSYWAITMUTEX_UTILS_H

#include <stdbool.h>

bool is_even(int number);

bool is_prime(int number);

int** create_ranges(int interval, int length);

void print_arr(int** arr, int length);

int* generate_params(int start, int end, int thread_number);

#endif //BUSYWAITMUTEX_UTILS_H
