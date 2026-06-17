#pragma once
#include <vector>
#include <string>

// Min Heap, Max Heap, Heap Sort
// MathEngine uses a MaxHeap as a priority queue for operator precedence


// Generic heap (IsMin=true -> min-heap, IsMin=false -> max-heap) 
template<bool IsMin>
class Heap {
public:
    Heap() = default;
    explicit Heap(const std::vector<double>& data);   // build in O(n)

    void   push(double val);
    double pop();                      // remove & return top
    double top() const;
    bool   empty() const { return data_.empty(); }
    int    size()  const { return (int)data_.size(); }

    std::string toString() const;      // level-order representation

private:
    std::vector<double> data_;
    void heapifyUp  (int i);
    void heapifyDown(int i);
    bool compare(double a, double b) const;   // true if a should be above b
};

using MinHeap = Heap<true>;
using MaxHeap = Heap<false>;

// Heap Sort (in-place, ascending)
namespace HeapSort {
    void sort(std::vector<double>& v);
}
