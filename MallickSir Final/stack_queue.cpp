#include "stack_queue.h"
#include <stdexcept>
#include <sstream>
#include <cstring>

// Stack
Stack::Stack(int capacity) : top_(-1), cap_(capacity) {
    data_ = new std::string[cap_];
}
Stack::~Stack() { delete[] data_; }

void Stack::grow() {
    int newCap = cap_ * 2;
    std::string* nd = new std::string[newCap];
    for (int i = 0; i <= top_; ++i) nd[i] = std::move(data_[i]);
    delete[] data_;
    data_ = nd;
    cap_  = newCap;
}

void Stack::push(const std::string& val) {
    if (top_ + 1 == cap_) grow();
    data_[++top_] = val;
}

std::string Stack::pop() {
    if (top_ < 0) throw std::underflow_error("Stack::pop on empty stack");
    return data_[top_--];
}

std::string Stack::peek() const {
    if (top_ < 0) throw std::underflow_error("Stack::peek on empty stack");
    return data_[top_];
}

bool Stack::empty() const { return top_ < 0; }
int  Stack::size()  const { return top_ + 1; }
void Stack::clear()       { top_ = -1; }

std::string Stack::toString() const {
    std::ostringstream oss;
    oss << "TOP → [";
    for (int i = top_; i >= 0; --i) {
        if (i != top_) oss << ", ";
        oss << data_[i];
    }
    oss << "]";
    return oss.str();
}

// Queue
Queue::Queue(int capacity) : head_(0), tail_(0), count_(0), cap_(capacity) {
    data_ = new std::string[cap_];
}
Queue::~Queue() { delete[] data_; }

void Queue::grow() {
    int newCap = cap_ * 2;
    std::string* nd = new std::string[newCap];
    for (int i = 0; i < count_; ++i)
        nd[i] = std::move(data_[(head_ + i) % cap_]);
    delete[] data_;
    data_ = nd;
    head_ = 0;
    tail_ = count_;
    cap_  = newCap;
}

void Queue::enqueue(const std::string& val) {
    if (count_ == cap_) grow();
    data_[tail_] = val;
    tail_ = (tail_ + 1) % cap_;
    ++count_;
}

std::string Queue::dequeue() {
    if (count_ == 0) throw std::underflow_error("Queue::dequeue on empty queue");
    std::string val = std::move(data_[head_]);
    head_ = (head_ + 1) % cap_;
    --count_;
    return val;
}

std::string Queue::front() const {
    if (count_ == 0) throw std::underflow_error("Queue::front on empty queue");
    return data_[head_];
}

bool Queue::empty() const { return count_ == 0; }
int  Queue::size()  const { return count_; }
void Queue::clear()       { head_ = tail_ = count_ = 0; }

std::string Queue::toString() const {
    std::ostringstream oss;
    oss << "FRONT → [";
    for (int i = 0; i < count_; ++i) {
        if (i) oss << ", ";
        oss << data_[(head_ + i) % cap_];
    }
    oss << "] ← BACK";
    return oss.str();
}
