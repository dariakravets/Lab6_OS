#include <iostream>
#include <chrono>
#include <cstdlib>
#include <pthread.h>

using namespace std;
using namespace std::chrono;

#define N 200
#define M 200
#define K 200

int A[N][M];
int B[M][K];
int C[N][K];

int num_threads = 10;

struct thread_data {
    int thread_id;
};

void* calculate(void* arg_ptr) {
    thread_data* data = (thread_data*) arg_ptr;
    int id = data->thread_id;
    int start = (N * K * id) / num_threads;
    int end = (N * K * (id + 1)) / num_threads;

    for (int i = start; i < end; i++) {
        int row = i / K;
        int col = i % K;
        int sum = 0;
        for (int j = 0; j < M; j++) {
            sum += A[row][j] * B[j][col];
        }
        C[row][col] = sum;
    }
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            A[i][j] = rand() % 100;
        }
      }
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            B[i][j] = rand() % 100;
        }
    }

    for (num_threads = 1; num_threads <= 20; num_threads++) {
        auto start_time = high_resolution_clock::now();

        pthread_t threads[num_threads];
        thread_data td[num_threads];

        for (int i = 0; i < num_threads; i++) {
            td[i].thread_id = i;
            pthread_create(&threads[i], NULL, calculate, (void*) &td[i]);
        }

        for (int i = 0; i < num_threads; i++) {
            pthread_join(threads[i], NULL);
        }

        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end_time - start_time);
        cout << "Time taken with " << num_threads << " threads: " << duration.count() << "ms\n";
    }

    return 0;
}
