#pragma once
#include <vector>
#include <string>

// Sorting Algorithms
// MathEngine uses these to sort variable lists and result sets.


class Sorting {
public:
    // O(n^2) sorts 
    static void bubbleSort   (std::vector<double>& v);
    static void insertionSort(std::vector<double>& v);
    static void selectionSort(std::vector<double>& v);

    // O(n log n) sorts 
    static void quickSort    (std::vector<double>& v);
    static void mergeSort    (std::vector<double>& v);

    //  Heap sort (uses Heap module) 
    static void heapSort     (std::vector<double>& v);   // defined in heap.cpp

    // Utility 
    static std::string toString(const std::vector<double>& v);

private:
    static void quickSortHelper(std::vector<double>& v, int lo, int hi);
    static int  partition      (std::vector<double>& v, int lo, int hi);
    static void mergeSortHelper(std::vector<double>& v, int lo, int hi);
    static void merge          (std::vector<double>& v, int lo, int mid, int hi);
};
