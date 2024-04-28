/*
#include <iostream>
#include <climits>
#include <cstdlib>
#include <chrono>
#include <omp.h>*/
#include<bits/stdc++.h>

using namespace std;

// Sequential Version
int findMinSequential(int arr[], int n) {
    int min_value = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (arr[i] < min_value) {
            min_value = arr[i];
        }
    }
    return min_value;
}

int findMaxSequential(int arr[], int n) {
    int max_value = INT_MIN;
    for (int i = 0; i < n; i++) {
        if (arr[i] > max_value) {
            max_value = arr[i];
        }
    }
    return max_value;
}

int calculateSumSequential(int arr[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

int calculateAverageSequential(int arr[], int n) {
    int sum = calculateSumSequential(arr, n);
    return sum / n;
}

// OpenMP Parallel Version
int findMinParallel(int arr[], int n) {
    int min_value = INT_MAX;
#pragma omp parallel for reduction(min: min_value)
    for (int i = 0; i < n; i++) {
        if (arr[i] < min_value) {
            min_value = arr[i];
        }
    }
    return min_value;
}

int findMaxParallel(int arr[], int n) {
    int max_value = INT_MIN;
#pragma omp parallel for reduction(max: max_value)
    for (int i = 0; i < n; i++) {
        if (arr[i] > max_value) {
            max_value = arr[i];
        }
    }
    return max_value;
}

int calculateSumParallel(int arr[], int n) {
    int sum = 0;
#pragma omp parallel for reduction(+: sum)
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

int calculateAverageParallel(int arr[], int n) {
    int sum = calculateSumParallel(arr, n);
    return sum / n;
}

int main() {
    int *arr, n;
    cout << "\nEnter total number of elements: ";
    cin >> n;

    int lb = 0;
    int ub = n;
    arr = new int[n];

    for (int i = 0; i < n; i++) {
        arr[i] = (rand() % (ub - lb + 1));
    }

    cout << "Generated array: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // Sequential version
    auto startSeqMin = chrono::steady_clock::now();
    int seqMin = findMinSequential(arr, n);
    auto endSeqMin = chrono::steady_clock::now();
    cout << "Sequential Min = " << seqMin << endl;
    cout << "Sequential Elapsed time for Min in microseconds: " << chrono::duration_cast<chrono::microseconds>(endSeqMin - startSeqMin).count() << " microsecs" << endl;

    auto startSeqMax = chrono::steady_clock::now();
    int seqMax = findMaxSequential(arr, n);
    auto endSeqMax = chrono::steady_clock::now();
    cout << "Sequential Max = " << seqMax << endl;
    cout << "Sequential Elapsed time for Max in microseconds: " << chrono::duration_cast<chrono::microseconds>(endSeqMax - startSeqMax).count() << " microsecs" << endl;

    auto startSeqSum = chrono::steady_clock::now();
    int seqSum = calculateSumSequential(arr, n);
    auto endSeqSum = chrono::steady_clock::now();
    cout << "Sequential Sum = " << seqSum << endl;
    cout << "Sequential Elapsed time for Sum in microseconds: " << chrono::duration_cast<chrono::microseconds>(endSeqSum - startSeqSum).count() << " microsecs" << endl;

    auto startSeqAvg = chrono::steady_clock::now();
    int seqAvg = calculateAverageSequential(arr, n);
    auto endSeqAvg = chrono::steady_clock::now();
    cout << "Sequential Average = " << seqAvg << endl;
    cout << "Sequential Elapsed time for Average in microseconds: " << chrono::duration_cast<chrono::microseconds>(endSeqAvg - startSeqAvg).count() << " microsecs" << endl;

    // OpenMP Parallel version
    auto startParallelMin = chrono::steady_clock::now();
    int parallelMin = findMinParallel(arr, n);
    auto endParallelMin = chrono::steady_clock::now();
    cout << "Parallel Min = " << parallelMin << endl;
    cout << "Parallel Elapsed time for Min in microseconds: " << chrono::duration_cast<chrono::microseconds>(endParallelMin - startParallelMin).count() << " microsecs" << endl;

    auto startParallelMax = chrono::steady_clock::now();
    int parallelMax = findMaxParallel(arr, n);
    auto endParallelMax = chrono::steady_clock::now();
    cout << "Parallel Max = " << parallelMax << endl;
    cout << "Parallel Elapsed time for Max in microseconds: " << chrono::duration_cast<chrono::microseconds>(endParallelMax - startParallelMax).count() << " microsecs" << endl;

    auto startParallelSum = chrono::steady_clock::now();
    int parallelSum = calculateSumParallel(arr, n);
    auto endParallelSum = chrono::steady_clock::now();
    cout << "Parallel Sum = " << parallelSum << endl;
    cout << "Parallel Elapsed time for Sum in microseconds: " << chrono::duration_cast<chrono::microseconds>(endParallelSum - startParallelSum).count() << " microsecs" << endl;

    auto startParallelAvg = chrono::steady_clock::now();
    int parallelAvg = calculateAverageParallel(arr, n);
    auto endParallelAvg = chrono::steady_clock::now();
    cout << "Parallel Average = " << parallelAvg << endl;
    cout << "Parallel Elapsed time for Average in microseconds: " << chrono::duration_cast<chrono::microseconds>(endParallelAvg - startParallelAvg).count() << " microsecs" << endl;

    // Calculate Speedup for each operation
    double speedupMin = static_cast<double>(chrono::duration_cast<chrono::microseconds>(endSeqMin - startSeqMin).count()) / static_cast<double>(chrono::duration_cast<chrono::microseconds>(endParallelMin - startParallelMin).count());
    double speedupMax = static_cast<double>(chrono::duration_cast<chrono::microseconds>(endSeqMax - startSeqMax).count()) / static_cast<double>(chrono::duration_cast<chrono::microseconds>(endParallelMax - startParallelMax).count());
    double speedupSum = static_cast<double>(chrono::duration_cast<chrono::microseconds>(endSeqSum - startSeqSum).count()) / static_cast<double>(chrono::duration_cast<chrono::microseconds>(endParallelSum - startParallelSum).count());
    double speedupAvg = static_cast<double>(chrono::duration_cast<chrono::microseconds>(endSeqAvg - startSeqAvg).count()) / static_cast<double>(chrono::duration_cast<chrono::microseconds>(endParallelAvg - startParallelAvg).count());

    cout << "\nSpeedup for Min = " << speedupMin << endl;
    cout << "Speedup for Max = " << speedupMax << endl;
    cout << "Speedup for Sum = " << speedupSum << endl;
    cout << "Speedup for Average = " << speedupAvg << endl;

    delete[] arr; // Don't forget to release the allocated memory
    return 0;
}
