#pragma once
#include <string>
#include <vector>
#include <functional>


// Binary Search Tree
// BST as the variable symbol table: name → value.


struct BSTNode {
    std::string key;
    double      val;
    BSTNode*    left  = nullptr;
    BSTNode*    right = nullptr;
    BSTNode(const std::string& k, double v) : key(k), val(v) {}
};

class BST {
public:
    BST() = default;
    ~BST();

    // Core operations 
    void   insert(const std::string& key, double val);
    bool   remove(const std::string& key);
    bool   search(const std::string& key, double& outVal) const;
    bool   contains(const std::string& key) const;
    void   update(const std::string& key, double val);    // insert or update

    // Traversals (callback receives key, val) 
    using Visitor = std::function<void(const std::string&, double)>;
    void inorder  (Visitor fn) const;
    void preorder (Visitor fn) const;
    void postorder(Visitor fn) const;

    // Utility
    int    size()    const { return size_; }
    bool   empty()   const { return root_ == nullptr; }
    void   clear();
    std::string toStringInorder() const;

    // Min / Max
    BSTNode* minNode() const;
    BSTNode* maxNode() const;

private:
    BSTNode* root_  = nullptr;
    int      size_  = 0;

    BSTNode* insertHelper (BSTNode* node, const std::string& k, double v);
    BSTNode* removeHelper (BSTNode* node, const std::string& k, bool& removed);
    BSTNode* minHelper    (BSTNode* node) const;
    void     clearHelper  (BSTNode* node);
    void     inHelper     (BSTNode* n, Visitor& fn) const;
    void     preHelper    (BSTNode* n, Visitor& fn) const;
    void     postHelper   (BSTNode* n, Visitor& fn) const;
};
