#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
    schelet pentru exercitiul 5
*/

#define NUM_THREADS 10

int* arr;
int array_size;

struct t_arg {
    int* arr;
    int size;
    time_t* d;
};

void* f(void* arg) {

    struct t_arg* args = (struct t_arg*)arg;
    int* arr = args->arr;
    int size = args->size;

    for (int i = 0; i < size; i++) {
        arr[i] += 100;
    }

    time(args->d);

    return arg;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Specificati dimensiunea array-ului\n");
        exit(-1);
    }

    array_size = atoi(argv[1]);

    arr = malloc(array_size * sizeof(int));
    for (int i = 0; i < array_size; i++) {
        arr[i] = i;
    }

    for (int i = 0; i < array_size; i++) {
        printf("%d", arr[i]);
        if (i != array_size - 1) {
            printf(" ");
        } else {
            printf("\n");
        }
    }

    // TODO: aceasta operatie va fi paralelizata
  	// for (int i = 0; i < array_size; i++) {
    //     arr[i] += 100;
    // }

    pthread_t threads[NUM_THREADS];
    int slice = array_size / NUM_THREADS;
    time_t t[NUM_THREADS];
    time_t start;
    time(&start);

    for (int i = 0; i < NUM_THREADS; i++) {
        struct t_arg* arg = malloc(sizeof(struct t_arg));
        arg->arr = arr + slice * i;
        arg->size = slice;
        arg->d = &t[i];
        int r = pthread_create(&threads[i], NULL, f, arg);
        if (r) {
            printf("Eroare la crearea thread-ului %d\n", i);
            exit(-1);
        }
    }
    // struct t_arg arg;
    // arg.arr = arr + (NUM_THREADS-1) * slice;
    // arg.size = rest;
    // int r = pthread_create(&threads[NUM_THREADS], NULL, f, &arg);
    // if (r) {
    //     printf("Eroare la crearea thread-ului %d\n", NUM_THREADS);
    //     exit(-1);
    // }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < array_size; i++) {
        printf("%d", arr[i]);
        if (i != array_size - 1) {
            printf(" ");
        } else {
            printf("\n");
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        printf("%.f\n", difftime(t[i], start));
    }

  	pthread_exit(NULL);
}
