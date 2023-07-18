#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <pthread.h>

using namespace std;

// Structure to hold thread-specific data
struct ThreadData {
    vector<vector<double>>* matrix;
    vector<double>* vectorPtr;
    vector<double> result;
    
    size_t start;
    size_t end;
};

// Function to be run by each thread
void* multiply_thread(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);

    for (size_t i = data->start; i < data->end; i++) {
        double sum = 0;
        for (size_t j = 0; j < data->vectorPtr->size(); j++) {
            sum += (*data->matrix)[i][j] * (*data->vectorPtr)[j];
        }
        data->result[i - data->start] = sum;
    }

    pthread_exit(NULL);
}

vector<double> matrix_vector_multiply(vector<vector<double>>& matrix, vector<double>& vectorptr) {
    // Create a result vector with the same number of rows as the matrix
    vector<double> result(matrix.size());

    // Determine the number of threads to use based on the number of entries in the vector
    size_t num_threads = matrix.size();

    // Create an array of thread IDs and thread-specific data
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];

    // Split the work among the threads
    size_t chunk_size = matrix.size() / num_threads;
    size_t remaining_rows = matrix.size() % num_threads;
    size_t start_row = 0;
    size_t end_row;

    for (size_t i = 0; i < num_threads; i++) {
        end_row = start_row + chunk_size;
        if (remaining_rows > 0) {
            end_row++;
            remaining_rows--;
        }

        thread_data[i].matrix = &matrix;
        thread_data[i].vectorPtr = &vectorptr;
        thread_data[i].start = start_row;
        thread_data[i].end = end_row;
        thread_data[i].result.resize(end_row - start_row);

        pthread_create(&threads[i], NULL, multiply_thread, (void*)&thread_data[i]);

        start_row = end_row;
    }

    // Wait for all threads to finish
    for (size_t i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Collect the results from each thread
    for (size_t i = 0; i < num_threads; i++) {
        size_t thread_result_size = thread_data[i].result.size();
        for (size_t j = 0; j < thread_result_size; j++) {
            result[j + thread_data[i].start] = thread_data[i].result[j];
        }
    }

    return result;
}

// int main() {
//     // Example usage
//     vector<vector<double>> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
//     vector<double> vectorptr = {2, 3, 4};
//     vector<double> result = matrix_vector_multiply(matrix, vectorptr);


//     // Print the result
//     for (size_t i = 0; i < result.size(); i++) {
//         cout << result[i] << " ";
//     }
//     cout << endl;


//     return 0;
// }
