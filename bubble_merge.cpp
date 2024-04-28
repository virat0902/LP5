/*#include <iostream>
#include <vector>
#include <omp.h>*/
#include <bits/stdc++.h>

using namespace std;

void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> leftArr(arr.begin() + left, arr.begin() + mid + 1);
    vector<int> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < leftArr.size() && j < rightArr.size()) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    while (i < leftArr.size()) {
        arr[k++] = leftArr[i++];
    }

    while (j < rightArr.size()) {
        arr[k++] = rightArr[j++];
    }
}

void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void parallelMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                parallelMergeSort(arr, left, mid);
            }

            #pragma omp section
            {
                parallelMergeSort(arr, mid + 1, right);
            }
        }

        merge(arr, left, mid, right);
    }
}

void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool swapped;

    for (int i = 0; i < n - 1; ++i) {
        swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        // If no two elements were swapped, the array is already sorted
        if (!swapped) {
            break;
        }
    }
}

void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool swapped;

    // Parallelized loop for each iteration of bubble sort
    for (int i = 0; i < n - 1; ++i) {
        swapped = false;

        // Parallelize inner loop using OpenMP
        #pragma omp parallel for reduction(| : swapped)
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        
        // If no two elements were swapped, the array is already sorted
        if (!swapped) {
            break;
        }
    }
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> arr(n);
    int ub = n;
    int lb = 0;
    for (int i = 0; i < n; ++i) {
  arr[i] = rand() % (ub - lb + 1) + lb;
}


    // Create a copy of the array for the sequential sort
    vector<int> arr_seq = arr;

    // Sequential merge sort
    auto startSeqMerge = chrono::steady_clock::now();
    sequentialMergeSort(arr_seq, 0, arr_seq.size() - 1);
    auto endSeqMerge = chrono::steady_clock::now();/*
    cout << "Sequential Merge Sort Result: ";
    for (int i = 0; i < n; i++) {
        cout << arr_seq[i] << " ";
    }
    cout << endl;*/
    cout << "Sequential Elapsed time for Merge sort in microseconds: " << chrono::duration_cast<chrono::microseconds>(endSeqMerge - startSeqMerge).count() << " microsecs" << endl;

    // Create a copy of the array for the parallel sort
    vector<int> arr_par = arr;

    // Parallel merge sort
    auto startParMerge = chrono::steady_clock::now();
    parallelMergeSort(arr_par, 0, arr_par.size() - 1);
    auto endParMerge = chrono::steady_clock::now();/*
    cout << "Parallel Merge Sort Result: ";
    for (int i = 0; i<n; i++) {
        cout << arr_par[i] << " ";
    }
    cout << endl;*/
    cout << "Parallel Elapsed time for Merge sort in microseconds: " << chrono::duration_cast<chrono::microseconds>(endParMerge - startParMerge).count() << " microsecs" << endl;
    
     // Create a copy of the array for the sequential sort
    vector<int> arr_seqb = arr;

    // Sequential bubble sort
    auto startSeqbubble = chrono::steady_clock::now();
    sequentialBubbleSort(arr_seqb);
    auto endSeqbubble = chrono::steady_clock::now();/*
    cout << "Sequential Bubble Sort Result: ";
    for (int i = 0; i<n; i++) {
        cout << arr_seqb[i] << " ";
    }
    cout << endl;*/
    cout << "Sequential Elapsed time for bubble sort in microseconds: " << chrono::duration_cast<chrono::microseconds>(endSeqbubble - startSeqbubble).count() << " microsecs" << endl;


    // Create a copy of the array for the parallel sort
    vector<int> arr_parb = arr;

    // Parallel bubble sort
    auto startParbubble = chrono::steady_clock::now();
    parallelBubbleSort(arr_parb);
    auto endParbubble = chrono::steady_clock::now();/*
    cout << "Parallel Bubble Sort Result: ";
    for (int i = 0; i<n; i++) {
        cout << arr_parb[i] << " ";
    }
    cout << endl;*/
    cout << "Parallel Elapsed time for Bubble sort in microseconds: " << chrono::duration_cast<chrono::microseconds>(endParbubble - startParbubble).count() << " microsecs" << endl;

    
    double speedupMerge = static_cast<double>(chrono::duration_cast<chrono::microseconds>(endSeqMerge - startSeqMerge).count()) / static_cast<double>(chrono::duration_cast<chrono::microseconds>(endParMerge - startParMerge).count());
    double speedupBubble = static_cast<double>(chrono::duration_cast<chrono::microseconds>(endSeqbubble - startSeqbubble).count()) / static_cast<double>(chrono::duration_cast<chrono::microseconds>(endParbubble - startParbubble).count());

    cout<<"Speedup for merge sort = "<<speedupMerge<<endl;
    cout<<"Speedup for Bubble sort = "<<speedupBubble<<endl;

    return 0;
}
