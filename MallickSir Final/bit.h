#pragma once
#include <vector>

// Binary Indexed Tree


class BIT {
public:
    explicit BIT(int n); // size n, all zeros
    explicit BIT(const std::vector<double>& init);  // build from array

    // Point update: add delta to position i (1-indexed) 
    void   update(int i, double delta);

    // Prefix sum [1 .. i] 
    double query(int i) const;

    // Range sum [l .. r] 
    double rangeQuery(int l, int r) const;

    // Point set (replaces value) 
    void   set(int i, double val);

    int    size() const { return n_; }
    void   reset();                       // set all to 0

private:
    int                  n_;
    std::vector<double>  tree_;
    double               point(int i) const;   // actual value at i
};
