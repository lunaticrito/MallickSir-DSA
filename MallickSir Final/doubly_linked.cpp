#include "doubly_linked.h"
#include <stdexcept>
#include <sstream>

// DoublyList
DoublyList::~DoublyList() {
    DNode* cur = head_;
    while (cur) { DNode* nx = cur->next; delete cur; cur = nx; }
}

void DoublyList::insertFirst(const std::string& val) {
    DNode* n = new DNode(val);
    n->next  = head_;
    if (head_) head_->prev = n;
    head_ = n;
    if (!tail_) tail_ = n;
    ++size_;
}

void DoublyList::insertLast(const std::string& val) {
    DNode* n = new DNode(val);
    n->prev  = tail_;
    if (tail_) tail_->next = n;
    tail_ = n;
    if (!head_) head_ = n;
    ++size_;
}

void DoublyList::insertAt(int pos, const std::string& val) {
    if (pos < 0 || pos > size_) throw std::out_of_range("DoublyList::insertAt");
    if (pos == 0)    { insertFirst(val); return; }
    if (pos == size_){ insertLast(val);  return; }
    DNode* cur = head_;
    for (int i = 0; i < pos; ++i) cur = cur->next;
    // Insert before cur
    DNode* n  = new DNode(val);
    n->next   = cur;
    n->prev   = cur->prev;
    cur->prev->next = n;
    cur->prev = n;
    ++size_;
}

bool DoublyList::deleteFirst() {
    if (!head_) return false;
    DNode* tmp = head_;
    head_ = head_->next;
    if (head_) head_->prev = nullptr;
    else       tail_ = nullptr;
    delete tmp;
    --size_;
    return true;
}

bool DoublyList::deleteLast() {
    if (!tail_) return false;
    DNode* tmp = tail_;
    tail_ = tail_->prev;
    if (tail_) tail_->next = nullptr;
    else       head_ = nullptr;
    delete tmp;
    --size_;
    return true;
}

bool DoublyList::deleteAt(int pos) {
    if (pos < 0 || pos >= size_) return false;
    if (pos == 0)       return deleteFirst();
    if (pos == size_-1) return deleteLast();
    DNode* cur = head_;
    for (int i = 0; i < pos; ++i) cur = cur->next;
    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;
    delete cur;
    --size_;
    return true;
}

bool DoublyList::deleteVal(const std::string& val) {
    DNode* cur = head_;
    int pos = 0;
    while (cur) {
        if (cur->data == val) return deleteAt(pos);
        cur = cur->next;
        ++pos;
    }
    return false;
}

int  DoublyList::size()  const { return size_; }
bool DoublyList::empty() const { return size_ == 0; }

std::string DoublyList::toString() const {
    std::ostringstream oss;
    oss << "NULL ↔ ";
    DNode* cur = head_;
    while (cur) {
        oss << cur->data;
        if (cur->next) oss << " ↔ ";
        cur = cur->next;
    }
    oss << " ↔ NULL";
    return oss.str();
}

std::string DoublyList::toStringRev() const {
    std::ostringstream oss;
    oss << "NULL ↔ ";
    DNode* cur = tail_;
    while (cur) {
        oss << cur->data;
        if (cur->prev) oss << " ↔ ";
        cur = cur->prev;
    }
    oss << " ↔ NULL (reversed)";
    return oss.str();
}

// DoublyCircular
DoublyCircular::~DoublyCircular() {
    if (!head_) return;
    DNode* tail = head_->prev;
    tail->next  = nullptr;     // break circle for linear delete
    DNode* cur  = head_;
    while (cur) { DNode* nx = cur->next; delete cur; cur = nx; }
}

void DoublyCircular::insertFirst(const std::string& val) {
    DNode* n = new DNode(val);
    if (!head_) {
        n->next = n; n->prev = n;
        head_ = n;
    } else {
        DNode* tail = head_->prev;
        n->next    = head_;
        n->prev    = tail;
        tail->next = n;
        head_->prev = n;
        head_ = n;
    }
    ++size_;
}

void DoublyCircular::insertLast(const std::string& val) {
    if (!head_) { insertFirst(val); return; }
    DNode* tail = head_->prev;
    DNode* n    = new DNode(val);
    n->prev     = tail;
    n->next     = head_;
    tail->next  = n;
    head_->prev = n;
    ++size_;
}

void DoublyCircular::insertAt(int pos, const std::string& val) {
    if (pos < 0 || pos > size_) throw std::out_of_range("DoublyCircular::insertAt");
    if (pos == 0)    { insertFirst(val); return; }
    if (pos == size_){ insertLast(val);  return; }
    DNode* cur = head_;
    for (int i = 0; i < pos; ++i) cur = cur->next;
    DNode* n   = new DNode(val);
    n->next    = cur;
    n->prev    = cur->prev;
    cur->prev->next = n;
    cur->prev       = n;
    ++size_;
}

bool DoublyCircular::deleteFirst() {
    if (!head_) return false;
    if (size_ == 1) { delete head_; head_ = nullptr; --size_; return true; }
    DNode* tail = head_->prev;
    DNode* tmp  = head_;
    head_       = head_->next;
    head_->prev = tail;
    tail->next  = head_;
    delete tmp;
    --size_;
    return true;
}

bool DoublyCircular::deleteLast() {
    if (!head_) return false;
    if (size_ == 1) { delete head_; head_ = nullptr; --size_; return true; }
    DNode* tail    = head_->prev;
    tail->prev->next = head_;
    head_->prev    = tail->prev;
    delete tail;
    --size_;
    return true;
}

bool DoublyCircular::deleteAt(int pos) {
    if (pos < 0 || pos >= size_) return false;
    if (pos == 0)       return deleteFirst();
    if (pos == size_-1) return deleteLast();
    DNode* cur = head_;
    for (int i = 0; i < pos; ++i) cur = cur->next;
    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;
    delete cur;
    --size_;
    return true;
}

int  DoublyCircular::size()  const { return size_; }
bool DoublyCircular::empty() const { return size_ == 0; }

std::string DoublyCircular::toString() const {
    if (!head_) return "(empty doubly circular)";
    std::ostringstream oss;
    DNode* cur = head_;
    do {
        oss << cur->data << " ↔ ";
        cur = cur->next;
    } while (cur != head_);
    oss << "(back to " << head_->data << ")";
    return oss.str();
}
