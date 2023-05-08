#include <iostream>
#include <pthread.h>
using namespace std;

#define N 20
#define M 20
#define K 20
#define NUM_THREADS 10

int A[N][M], B[M][K], C[N][K];
pthread_mutex_t lock;

void* calculate(void* arg_ptr) {
    int id = *((int*)arg_ptr);
    int start = (N * K * id) / NUM_THREADS;
    int end = (N * K * (id + 1)) / NUM_THREADS;

    for (int i = start; i < end; i++) {
        int row = i / K;
        int col = i % K;
        int sum = 0;
        for (int j = 0; j < M; j++) {
            sum += A[row][j] * B[j][col];
        }

        pthread_mutex_lock(&lock);
        C[row][col] += sum;
        cout << "[" << row << "," << col << "]=" << C[row][col] << endl;
        pthread_mutex_unlock(&lock);
    }

    pthread_exit(NULL);
}

int main() {
    // Initialize matrices A and B
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            A[i][j] = i + j;
            B[j][i] = i - j;
        }
    }

    // Initialize mutex
    pthread_mutex_init(&lock, NULL);
   // Create threads
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, calculate, (void*)&thread_args[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy mutex
    pthread_mutex_destroy(&lock);

    return 0;
}
