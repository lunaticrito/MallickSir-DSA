#include "bst.h"
#include <sstream>

BST::~BST() { clear(); }

void BST::clear() { clearHelper(root_); root_ = nullptr; size_ = 0; }

void BST::clearHelper(BSTNode* n) {
    if (!n) return;
    clearHelper(n->left);
    clearHelper(n->right);
    delete n;
}

BSTNode* BST::insertHelper(BSTNode* n, const std::string& k, double v) {
    if (!n) { ++size_; return new BSTNode(k, v); }
    if (k < n->key)      n->left  = insertHelper(n->left,  k, v);
    else if (k > n->key) n->right = insertHelper(n->right, k, v);
    else                 n->val   = v;    // update
    return n;
}

void BST::insert(const std::string& key, double val) {
    root_ = insertHelper(root_, key, val);
}

void BST::update(const std::string& key, double val) { insert(key, val); }

BSTNode* BST::minHelper(BSTNode* n) const {
    while (n && n->left) n = n->left;
    return n;
}

BSTNode* BST::removeHelper(BSTNode* n, const std::string& k, bool& removed) {
    if (!n) return nullptr;
    if (k < n->key)      n->left  = removeHelper(n->left,  k, removed);
    else if (k > n->key) n->right = removeHelper(n->right, k, removed);
    else {
        removed = true;
        if (!n->left)  { BSTNode* r = n->right; delete n; --size_; return r; }
        if (!n->right) { BSTNode* l = n->left;  delete n; --size_; return l; }
        // Two children: replace with inorder successor
        BSTNode* succ = minHelper(n->right);
        n->key = succ->key;
        n->val = succ->val;
        bool dummy = false;
        n->right = removeHelper(n->right, succ->key, dummy);
        if (dummy) ++size_;   // removeHelper will decrement again
    }
    return n;
}

bool BST::remove(const std::string& key) {
    bool removed = false;
    root_ = removeHelper(root_, key, removed);
    return removed;
}

bool BST::search(const std::string& key, double& outVal) const {
    BSTNode* cur = root_;
    while (cur) {
        if (key == cur->key)      { outVal = cur->val; return true; }
        else if (key < cur->key)  cur = cur->left;
        else                      cur = cur->right;
    }
    return false;
}

bool BST::contains(const std::string& key) const {
    double dummy;
    return search(key, dummy);
}

void BST::inHelper  (BSTNode* n, Visitor& fn) const { if (!n) return; inHelper  (n->left,  fn); fn(n->key, n->val); inHelper  (n->right, fn); }
void BST::preHelper (BSTNode* n, Visitor& fn) const { if (!n) return; fn(n->key, n->val); preHelper (n->left,  fn); preHelper (n->right, fn); }
void BST::postHelper(BSTNode* n, Visitor& fn) const { if (!n) return; postHelper(n->left,  fn); postHelper(n->right, fn); fn(n->key, n->val); }

void BST::inorder  (Visitor fn) const { inHelper  (root_, fn); }
void BST::preorder (Visitor fn) const { preHelper (root_, fn); }
void BST::postorder(Visitor fn) const { postHelper(root_, fn); }

BSTNode* BST::minNode() const { return minHelper(root_); }
BSTNode* BST::maxNode() const {
    BSTNode* n = root_;
    while (n && n->right) n = n->right;
    return n;
}

std::string BST::toStringInorder() const {
    std::ostringstream oss;
    bool first = true;
    inorder([&](const std::string& k, double v) {
        if (!first) oss << ", ";
        oss << k << "=" << v;
        first = false;
    });
    return oss.str();
}
