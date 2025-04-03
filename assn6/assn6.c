#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 20

typedef struct {
    int number;
    int *factors;
    int factor_count;
} FactorResult;

int input_numbers[BUFFER_SIZE];
int input_write_index = 0, input_read_index = 0;
sem_t input_slots_available, input_items_available;

FactorResult *factor_results[BUFFER_SIZE];
int result_write_index = 0, result_read_index = 0;
sem_t result_slots_available, result_items_available;

pthread_mutex_t input_index_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t result_index_mutex = PTHREAD_MUTEX_INITIALIZER;

// prime fac. function
void compute_prime_factors(int num, int **factors, int *count) {
    int *temp_factors = malloc(31 * sizeof(int));
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
    if (num > 1) temp_factors[index++] = num;

    *factors = temp_factors;
    *count = index;
}

//computing thee thread
void *compute_thread_function(void *arg) {
    while (1) {
        sem_wait(&input_items_available);

        pthread_mutex_lock(&input_index_mutex);
        int num = input_numbers[input_read_index];
        input_read_index = (input_read_index + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&input_index_mutex);

        sem_post(&input_slots_available);

        if (num == -1) break;

        FactorResult *result = malloc(sizeof(FactorResult));
        result->number = num;
        compute_prime_factors(num, &result->factors, &result->factor_count);

        sem_wait(&result_slots_available);
        pthread_mutex_lock(&result_index_mutex);
        factor_results[result_write_index] = result;
        result_write_index = (result_write_index + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&result_index_mutex);
        sem_post(&result_items_available);
    }
    pthread_exit(NULL);
}

//The outpu
void *output_thread_function(void *arg) {
    printf("Assignment 6: Thread Synchronization by Jakob Hodgson\n");
    while (1) {
        sem_wait(&result_items_available);

        pthread_mutex_lock(&result_index_mutex);
        FactorResult *result = factor_results[result_read_index];
        result_read_index = (result_read_index + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&result_index_mutex);

        sem_post(&result_slots_available);

        if (result == NULL) break;

        printf("%d:", result->number);
        for (int i = 0; i < result->factor_count; i++) {
            printf(" %d", result->factors[i]);
        }
        printf("\n");

        free(result->factors);
        free(result);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./assn6 <number> ...\n");
        return 1;
    }

    sem_init(&input_slots_available, 0, BUFFER_SIZE);
    sem_init(&input_items_available, 0, 0);
    sem_init(&result_slots_available, 0, BUFFER_SIZE);
    sem_init(&result_items_available, 0, 0);

    pthread_t compute_thread, output_thread;

    pthread_create(&compute_thread, NULL, compute_thread_function, NULL);
    pthread_create(&output_thread, NULL, output_thread_function, NULL);

    //main thread
    for (int i = 1; i < argc; i++) {
        int number = atoi(argv[i]);

        sem_wait(&input_slots_available);
        pthread_mutex_lock(&input_index_mutex);
        input_numbers[input_write_index] = number;
        input_write_index = (input_write_index + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&input_index_mutex);
        sem_post(&input_items_available);
    }
    sem_wait(&input_slots_available);
    pthread_mutex_lock(&input_index_mutex);
    input_numbers[input_write_index] = -1;
    input_write_index = (input_write_index + 1) % BUFFER_SIZE;
    pthread_mutex_unlock(&input_index_mutex);
    sem_post(&input_items_available);

    pthread_join(compute_thread, NULL);

    sem_wait(&result_slots_available);
    pthread_mutex_lock(&result_index_mutex);
    factor_results[result_write_index] = NULL;
    result_write_index = (result_write_index + 1) % BUFFER_SIZE;
    pthread_mutex_unlock(&result_index_mutex);
    sem_post(&result_items_available);

    pthread_join(output_thread, NULL);

    sem_destroy(&input_slots_available);
    sem_destroy(&input_items_available);
    sem_destroy(&result_slots_available);
    sem_destroy(&result_items_available);

    return 0;
}