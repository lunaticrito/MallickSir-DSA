#include "array_ops.h"
#include <stdexcept>
#include <sstream>

void ArrayOps::insertFirst(const std::string& val) {
    arr_.insert(arr_.begin(), val);
}

void ArrayOps::insertLast(const std::string& val) {
    arr_.push_back(val);
}

void ArrayOps::insertAt(int index, const std::string& val) {
    if (index < 0 || index > (int)arr_.size())
        throw std::out_of_range("ArrayOps::insertAt: index out of range");
    arr_.insert(arr_.begin() + index, val);
}

bool ArrayOps::deleteFirst() {
    if (arr_.empty()) return false;
    arr_.erase(arr_.begin());
    return true;
}

bool ArrayOps::deleteLast() {
    if (arr_.empty()) return false;
    arr_.pop_back();
    return true;
}

bool ArrayOps::deleteAt(int index) {
    if (index < 0 || index >= (int)arr_.size()) return false;
    arr_.erase(arr_.begin() + index);
    return true;
}

const std::string& ArrayOps::get(int index) const {
    if (index < 0 || index >= (int)arr_.size())
        throw std::out_of_range("ArrayOps::get: index out of range");
    return arr_[index];
}

int  ArrayOps::size()  const { return (int)arr_.size(); }
bool ArrayOps::empty() const { return arr_.empty(); }
void ArrayOps::clear()       { arr_.clear(); }

std::string ArrayOps::toString() const {
    std::ostringstream oss;
    oss << "[";
    for (int i = 0; i < (int)arr_.size(); ++i) {
        if (i) oss << ", ";
        oss << arr_[i];
    }
    oss << "]";
    return oss.str();
}
