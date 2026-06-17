#pragma once
#include <string>

// Doubly Linked List 
// Undo-redo buffer

struct DNode {
    std::string data;
    DNode* prev = nullptr;
    DNode* next = nullptr;
    explicit DNode(const std::string& d) : data(d) {}
};

// Doubly Linked List 
class DoublyList {
public:
    DoublyList() = default;
    ~DoublyList();

    void insertFirst(const std::string& val);
    void insertLast (const std::string& val);
    void insertAt   (int pos, const std::string& val);  // 0-based

    bool deleteFirst();
    bool deleteLast();
    bool deleteAt(int pos);
    bool deleteVal(const std::string& val);

    int         size()     const;
    bool        empty()    const;
    std::string toString() const;          // forward
    std::string toStringRev() const;       // backward
    DNode*      head() const { return head_; }
    DNode*      tail() const { return tail_; }

private:
    DNode* head_ = nullptr;
    DNode* tail_ = nullptr;
    int    size_ = 0;
};

// Doubly Circular Linked List 
class DoublyCircular {
public:
    DoublyCircular() = default;
    ~DoublyCircular();

    void insertFirst(const std::string& val);
    void insertLast (const std::string& val);
    void insertAt   (int pos, const std::string& val);

    bool deleteFirst();
    bool deleteLast();
    bool deleteAt(int pos);

    int         size()     const;
    bool        empty()    const;
    std::string toString() const;
    DNode*      head() const { return head_; }

private:
    DNode* head_ = nullptr;
    int    size_ = 0;
};
