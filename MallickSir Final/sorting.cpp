#include "sorting.h"
#include "heap.h"
#include <sstream>
#include <iomanip>

// Bubble Sort 
void Sorting::bubbleSort(std::vector<double>& v) {
    int n = (int)v.size();
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (v[j] > v[j+1]) { std::swap(v[j], v[j+1]); swapped = true; }
        }
        if (!swapped) break;   // already sorted
    }
}

// Insertion Sort 
void Sorting::insertionSort(std::vector<double>& v) {
    int n = (int)v.size();
    for (int i = 1; i < n; ++i) {
        double key = v[i];
        int j = i - 1;
        while (j >= 0 && v[j] > key) { v[j+1] = v[j]; --j; }
        v[j+1] = key;
    }
}

// Selection Sort 
void Sorting::selectionSort(std::vector<double>& v) {
    int n = (int)v.size();
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < n; ++j)
            if (v[j] < v[minIdx]) minIdx = j;
        std::swap(v[i], v[minIdx]);
    }
}

// Quick Sort 
int Sorting::partition(std::vector<double>& v, int lo, int hi) {
    double pivot = v[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; ++j)
        if (v[j] <= pivot) std::swap(v[++i], v[j]);
    std::swap(v[i+1], v[hi]);
    return i + 1;
}

void Sorting::quickSortHelper(std::vector<double>& v, int lo, int hi) {
    if (lo < hi) {
        int p = partition(v, lo, hi);
        quickSortHelper(v, lo, p - 1);
        quickSortHelper(v, p + 1, hi);
    }
}

void Sorting::quickSort(std::vector<double>& v) {
    if (!v.empty()) quickSortHelper(v, 0, (int)v.size() - 1);
}

// Merge Sort 
void Sorting::merge(std::vector<double>& v, int lo, int mid, int hi) {
    std::vector<double> L(v.begin() + lo, v.begin() + mid + 1);
    std::vector<double> R(v.begin() + mid + 1, v.begin() + hi + 1);
    int i = 0, j = 0, k = lo;
    while (i < (int)L.size() && j < (int)R.size())
        v[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < (int)L.size()) v[k++] = L[i++];
    while (j < (int)R.size()) v[k++] = R[j++];
}

void Sorting::mergeSortHelper(std::vector<double>& v, int lo, int hi) {
    if (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        mergeSortHelper(v, lo, mid);
        mergeSortHelper(v, mid + 1, hi);
        merge(v, lo, mid, hi);
    }
}

void Sorting::mergeSort(std::vector<double>& v) {
    if (!v.empty()) mergeSortHelper(v, 0, (int)v.size() - 1);
}

// Heap Sort (delegates to HeapSort module) 
void Sorting::heapSort(std::vector<double>& v) {
    HeapSort::sort(v);
}

//  Utility 
std::string Sorting::toString(const std::vector<double>& v) {
    std::ostringstream oss;
    oss << "[";
    for (int i = 0; i < (int)v.size(); ++i) {
        if (i) oss << ", ";
        oss << std::setprecision(6) << v[i];
    }
    oss << "]";
    return oss.str();
}
