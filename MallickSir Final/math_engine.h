#pragma once
#include <string>
#include <vector>


// MathEngine Core API
// FOR UI INTEGRATION


struct EvalResult {
    bool        ok      = false;
    double      value   = 0.0;
    std::string postfix;
    std::string prefix;
    std::string error;
};

struct DiffResult {
    bool        ok          = false;
    std::string original;
    std::string derivative;
    std::string error;
};

struct IntegResult {
    bool        ok              = false;
    std::string original;
    std::string antiderivative;
    std::string error;
};

struct VariableInfo {
    std::string name;
    double      value;
};

class MathEngine {
public:
    MathEngine();
    ~MathEngine();

    // Expression evaluation 
    EvalResult evaluate(const std::string& expr);

    // Symbolic differentiation (CAS) 
    DiffResult differentiate(const std::string& expr, const std::string& wrt);

    // Symbolic integration (CAS) 
    IntegResult integrate(const std::string& expr, const std::string& wrt);

    // Variable management (BST-backed) 
    void   setVar(const std::string& name, double val);
    bool   getVar(const std::string& name, double& val) const;
    void   deleteVar(const std::string& name);
    std::vector<VariableInfo> listVars() const;       // sorted alphabetically

    // History (singly linked list) 
    void   pushHistory(const std::string& expr);
    std::string getHistory(int index) const;          // 0 = most recent
    int    historySize() const;
    std::string historyToString() const;

    //Undo/Redo buffer (doubly linked list) 
    bool   canUndo() const;
    bool   canRedo() const;
    std::string undo();
    std::string redo();

    // Token buffer (array ops) 
    std::vector<std::string> tokenise(const std::string& expr) const;

    // Sorting utilities 
    enum class SortAlgo { Bubble, Insertion, Selection, Quick, Merge, Heap };
    std::vector<double> sortValues(std::vector<double> vals, SortAlgo algo) const;

    // Graph: multi-expression dependency evaluation 
    //  addDependency("y", "x")  means y depends on x (x must eval first)
    void   addDependency(const std::string& target, const std::string& dep);
    std::vector<std::string> evalOrder() const;  // topological sort

    // BIT: prefix statistics over last N results 
    double prefixSum(int upToIndex) const;   // sum of results[1..upToIndex]
    double rangeSum (int from, int to) const;

    // DSA demos (for terminal menu) 
    // Each returns a multi-line string describing what was done.
    std::string demoArrayOps()      const;
    std::string demoStackQueue()    const;
    std::string demoLinkedLists()   const;
    std::string demoDoublyLists()   const;
    std::string demoSorting()       const;
    std::string demoBST()           const;
    std::string demoBIT()           const;
    std::string demoHeap()          const;
    std::string demoGraph()         const;

private:
    struct Impl;
    Impl* impl_;    // Pimpl - keeps DSA headers out of the UI layer
};
