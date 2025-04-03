#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int number;
    int *factors;
    int count;
} ThreadData;

void find_prime_factors(int num, int **factors, int *count) {
    int *temp_factors = malloc(31 * sizeof(int)); // max 31 factors
    int index = 0;

    while (num % 2 == 0) {
        temp_factors[index++] = 2;
        num /= 2;
    }
    for (int i = 3; i * i <= num; i += 2) {
        while (num % i == 0) {
            temp_factors[index++] = i;
            num /= i;
        }
    }
    if (num > 1) {
        temp_factors[index++] = num;
    }
    *factors = temp_factors;
    *count = index;
}

void *factorize(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    find_prime_factors(data->number, &data->factors, &data->count);
    pthread_exit((void *)data);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Assignment 5: The Multithreaded Factor Finder by Jakob Hodgson\n");
        printf("Usage: ./assn5 <number to factor>...\n");
        return 1;
    }

    printf("Assignment 5: The Multithreaded Factor Finder by Jakob Hodgson\n");

    int num_threads = argc - 1;
    pthread_t threads[num_threads];
    ThreadData *thread_data[num_threads];

    for (int i = 0; i < num_threads; i++) {
        thread_data[i] = malloc(sizeof(ThreadData));
        thread_data[i]->number = atoi(argv[i + 1]);
        thread_data[i]->factors = NULL;
        thread_data[i]->count = 0;
        pthread_create(&threads[i], NULL, factorize, (void *)thread_data[i]);
    }

    // combing the threads and printing out the results...
    for (int i = 0; i < num_threads; i++) {
        ThreadData *result;
        pthread_join(threads[i], (void **)&result);
        printf("%d:", result->number);
        for (int j = 0; j < result->count; j++) {
            printf(" %d", result->factors[j]);
        }
        printf("\n");
        free(result->factors);
        free(result);
    }

    return 0;
}
