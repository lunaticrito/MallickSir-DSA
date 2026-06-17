#include "bit.h"
#include <stdexcept>

BIT::BIT(int n) : n_(n), tree_(n + 1, 0.0) {}

BIT::BIT(const std::vector<double>& init) : n_((int)init.size()), tree_(init.size() + 1, 0.0) {
    for (int i = 1; i <= n_; ++i) update(i, init[i-1]);
}

void BIT::update(int i, double delta) {
    for (; i <= n_; i += i & (-i))
        tree_[i] += delta;
}

double BIT::query(int i) const {
    double s = 0;
    for (; i > 0; i -= i & (-i))
        s += tree_[i];
    return s;
}

double BIT::rangeQuery(int l, int r) const {
    if (l > r) return 0;
    return query(r) - query(l - 1);
}

double BIT::point(int i) const {
    return rangeQuery(i, i);
}

void BIT::set(int i, double val) {
    double cur = point(i);
    update(i, val - cur);
}

void BIT::reset() {
    std::fill(tree_.begin(), tree_.end(), 0.0);
}
