#include "singly_linked.h"
#include <stdexcept>
#include <sstream>

// SinglyList
SinglyList::~SinglyList() {
    SNode* cur = head_;
    while (cur) { SNode* nx = cur->next; delete cur; cur = nx; }
}

void SinglyList::insertFirst(const std::string& val) {
    SNode* n = new SNode(val);
    n->next  = head_;
    head_    = n;
    ++size_;
}

void SinglyList::insertLast(const std::string& val) {
    SNode* n = new SNode(val);
    if (!head_) { head_ = n; ++size_; return; }
    SNode* cur = head_;
    while (cur->next) cur = cur->next;
    cur->next = n;
    ++size_;
}

void SinglyList::insertAt(int pos, const std::string& val) {
    if (pos < 0 || pos > size_) throw std::out_of_range("SinglyList::insertAt");
    if (pos == 0) { insertFirst(val); return; }
    SNode* cur = head_;
    for (int i = 0; i < pos - 1; ++i) cur = cur->next;
    SNode* n  = new SNode(val);
    n->next   = cur->next;
    cur->next = n;
    ++size_;
}

bool SinglyList::deleteFirst() {
    if (!head_) return false;
    SNode* tmp = head_;
    head_ = head_->next;
    delete tmp;
    --size_;
    return true;
}

bool SinglyList::deleteLast() {
    if (!head_) return false;
    if (!head_->next) { delete head_; head_ = nullptr; --size_; return true; }
    SNode* cur = head_;
    while (cur->next->next) cur = cur->next;
    delete cur->next;
    cur->next = nullptr;
    --size_;
    return true;
}

bool SinglyList::deleteAt(int pos) {
    if (pos < 0 || pos >= size_) return false;
    if (pos == 0) return deleteFirst();
    SNode* cur = head_;
    for (int i = 0; i < pos - 1; ++i) cur = cur->next;
    SNode* tmp = cur->next;
    cur->next  = tmp->next;
    delete tmp;
    --size_;
    return true;
}

bool SinglyList::deleteVal(const std::string& val) {
    if (!head_) return false;
    if (head_->data == val) return deleteFirst();
    SNode* cur = head_;
    while (cur->next && cur->next->data != val) cur = cur->next;
    if (!cur->next) return false;
    SNode* tmp = cur->next;
    cur->next  = tmp->next;
    delete tmp;
    --size_;
    return true;
}

int  SinglyList::size()  const { return size_; }
bool SinglyList::empty() const { return size_ == 0; }

std::string SinglyList::toString() const {
    std::ostringstream oss;
    SNode* cur = head_;
    while (cur) {
        oss << cur->data;
        if (cur->next) oss << " → ";
        cur = cur->next;
    }
    oss << " → NULL";
    return oss.str();
}

// SinglyCircular
SinglyCircular::~SinglyCircular() {
    if (!tail_) return;
    SNode* head = tail_->next;
    SNode* cur  = head;
    do {
        SNode* nx = cur->next;
        delete cur;
        cur = nx;
    } while (cur != head);
}

void SinglyCircular::insertFirst(const std::string& val) {
    SNode* n = new SNode(val);
    if (!tail_) { n->next = n; tail_ = n; }
    else        { n->next = tail_->next; tail_->next = n; }
    ++size_;
}

void SinglyCircular::insertLast(const std::string& val) {
    insertFirst(val);         // insert after tail
    tail_ = tail_->next;      // advance tail to new node
}

void SinglyCircular::insertAt(int pos, const std::string& val) {
    if (pos < 0 || pos > size_) throw std::out_of_range("SinglyCircular::insertAt");
    if (pos == 0)     { insertFirst(val); return; }
    if (pos == size_) { insertLast(val);  return; }
    SNode* cur = tail_->next;  // head
    for (int i = 0; i < pos - 1; ++i) cur = cur->next;
    SNode* n  = new SNode(val);
    n->next   = cur->next;
    cur->next = n;
    ++size_;
}

bool SinglyCircular::deleteFirst() {
    if (!tail_) return false;
    SNode* head = tail_->next;
    if (head == tail_) { delete head; tail_ = nullptr; --size_; return true; }
    tail_->next = head->next;
    delete head;
    --size_;
    return true;
}

bool SinglyCircular::deleteLast() {
    if (!tail_) return false;
    SNode* head = tail_->next;
    if (head == tail_) { delete head; tail_ = nullptr; --size_; return true; }
    SNode* cur = head;
    while (cur->next != tail_) cur = cur->next;
    cur->next = tail_->next;
    delete tail_;
    tail_ = cur;
    --size_;
    return true;
}

bool SinglyCircular::deleteAt(int pos) {
    if (pos < 0 || pos >= size_) return false;
    if (pos == 0)       return deleteFirst();
    if (pos == size_-1) return deleteLast();
    SNode* cur = tail_->next;
    for (int i = 0; i < pos - 1; ++i) cur = cur->next;
    SNode* tmp = cur->next;
    cur->next  = tmp->next;
    delete tmp;
    --size_;
    return true;
}

int  SinglyCircular::size()  const { return size_; }
bool SinglyCircular::empty() const { return size_ == 0; }

std::string SinglyCircular::toString() const {
    if (!tail_) return "(empty circular list)";
    std::ostringstream oss;
    SNode* head = tail_->next;
    SNode* cur  = head;
    do {
        oss << cur->data << " → ";
        cur = cur->next;
    } while (cur != head);
    oss << "(back to " << head->data << ")";
    return oss.str();
}
