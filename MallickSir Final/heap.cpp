#include "heap.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>

//Template instantiation
template<bool IsMin>
bool Heap<IsMin>::compare(double a, double b) const {
    return IsMin ? (a < b) : (a > b);
}

template<bool IsMin>
Heap<IsMin>::Heap(const std::vector<double>& data) : data_(data) {
    // Build heap in O(n)
    for (int i = (int)data_.size() / 2 - 1; i >= 0; --i)
        heapifyDown(i);
}

template<bool IsMin>
void Heap<IsMin>::heapifyUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (compare(data_[i], data_[parent])) {
            std::swap(data_[i], data_[parent]);
            i = parent;
        } else break;
    }
}

template<bool IsMin>
void Heap<IsMin>::heapifyDown(int i) {
    int n = (int)data_.size();
    while (true) {
        int best  = i;
        int left  = 2*i + 1;
        int right = 2*i + 2;
        if (left  < n && compare(data_[left],  data_[best])) best = left;
        if (right < n && compare(data_[right], data_[best])) best = right;
        if (best == i) break;
        std::swap(data_[i], data_[best]);
        i = best;
    }
}

template<bool IsMin>
void Heap<IsMin>::push(double val) {
    data_.push_back(val);
    heapifyUp((int)data_.size() - 1);
}

template<bool IsMin>
double Heap<IsMin>::pop() {
    if (data_.empty()) throw std::underflow_error("Heap::pop on empty heap");
    double top = data_[0];
    data_[0] = data_.back();
    data_.pop_back();
    if (!data_.empty()) heapifyDown(0);
    return top;
}

template<bool IsMin>
double Heap<IsMin>::top() const {
    if (data_.empty()) throw std::underflow_error("Heap::top on empty heap");
    return data_[0];
}

template<bool IsMin>
std::string Heap<IsMin>::toString() const {
    std::ostringstream oss;
    oss << (IsMin ? "MinHeap" : "MaxHeap") << "[";
    for (int i = 0; i < (int)data_.size(); ++i) {
        if (i) oss << ", ";
        oss << std::setprecision(4) << data_[i];
    }
    oss << "]";
    return oss.str();
}

// Explicit instantiation for both specialisations
template class Heap<true>;
template class Heap<false>;

// Heap Sort (in-place, ascending) 
namespace HeapSort {
    void sort(std::vector<double>& v) {
        int n = (int)v.size();
        // Build max-heap in-place
        for (int i = n/2 - 1; i >= 0; --i) {
            // sift down i in [0..n)
            int j = i;
            while (true) {
                int best = j, l = 2*j+1, r = 2*j+2;
                if (l < n && v[l] > v[best]) best = l;
                if (r < n && v[r] > v[best]) best = r;
                if (best == j) break;
                std::swap(v[j], v[best]);
                j = best;
            }
        }
        // Extract elements
        for (int i = n - 1; i > 0; --i) {
            std::swap(v[0], v[i]);
            int j = 0, sz = i;
            while (true) {
                int best = j, l = 2*j+1, r = 2*j+2;
                if (l < sz && v[l] > v[best]) best = l;
                if (r < sz && v[r] > v[best]) best = r;
                if (best == j) break;
                std::swap(v[j], v[best]);
                j = best;
            }
        }
    }
}
