#pragma once
#include <string>
#include <stdexcept>


// Stack & Queue 
// Used by ExpressionEngine for shunting-yard parsing

//  Stack 
class Stack {
public:
    explicit Stack(int capacity = 256);
    ~Stack();

    void        push(const std::string& val);
    std::string pop();
    std::string peek() const;
    bool        empty() const;
    int         size()  const;
    void        clear();
    std::string toString() const;      // top -> bottom

private:
    std::string* data_;
    int  top_;
    int  cap_;
    void grow();
};

//  Queue 
class Queue {
public:
    explicit Queue(int capacity = 256);
    ~Queue();

    void        enqueue(const std::string& val);
    std::string dequeue();
    std::string front() const;
    bool        empty() const;
    int         size()  const;
    void        clear();
    std::string toString() const;      // front -> back

private:
    std::string* data_;
    int  head_, tail_, count_, cap_;
    void grow();
};
