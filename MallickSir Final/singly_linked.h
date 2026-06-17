#pragma once
#include <string>


// Singly Linked List & Singly Circular Linked List
// to store expression history.


// Singly Linked List 
struct SNode {
    std::string data;
    SNode* next = nullptr;
    explicit SNode(const std::string& d) : data(d) {}
};

class SinglyList {
public:
    SinglyList() = default;
    ~SinglyList();

    void insertFirst(const std::string& val);
    void insertLast (const std::string& val);
    void insertAt   (int pos, const std::string& val);  // 0-based

    bool deleteFirst();
    bool deleteLast();
    bool deleteAt(int pos);
    bool deleteVal(const std::string& val);

    int         size()  const;
    bool        empty() const;
    std::string toString() const;
    SNode*      head()  const { return head_; }

private:
    SNode* head_ = nullptr;
    int    size_ = 0;
};

// Singly Circular Linked List 
class SinglyCircular {
public:
    SinglyCircular() = default;
    ~SinglyCircular();

    void insertFirst(const std::string& val);
    void insertLast (const std::string& val);
    void insertAt   (int pos, const std::string& val);

    bool deleteFirst();
    bool deleteLast();
    bool deleteAt(int pos);

    int         size()  const;
    bool        empty() const;
    std::string toString() const;   // shows circular marker
    SNode*      tail() const { return tail_; }

private:
    SNode* tail_ = nullptr;          // tail->next == head
    int    size_ = 0;
};
