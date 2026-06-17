#include "math_engine.h"
#include "symbolic.h"
#include "array_ops.h"
#include "stack_queue.h"
#include "expression_engine.h"
#include "singly_linked.h"
#include "doubly_linked.h"
#include "sorting.h"
#include "bst.h"
#include "bit.h"
#include "heap.h"
#include "graph.h"

#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>


// Pimpl - all DSA state is here; the UI never sees these headers

struct MathEngine::Impl
{
    BST symTable;        // variable store
    SinglyList history;  // expression history
    DoublyList undoRedo; // undo/redo buffer
    DNode *undoCursor = nullptr;
    Graph depGraph; // expression dependency graph
    BIT *resultBIT = nullptr;
    std::vector<double> results; // raw result store for BIT

    // Token buffer (array ops)
    ArrayOps tokenBuf;

    ~Impl() { delete resultBIT; }

    void recordResult(double val)
    {
        results.push_back(val);
        // Rebuild BIT each time (simple; can optimise)
        delete resultBIT;
        resultBIT = new BIT(results);
    }
};

MathEngine::MathEngine() : impl_(new Impl()) {}
MathEngine::~MathEngine() { delete impl_; }

// Evaluate 

EvalResult MathEngine::evaluate(const std::string &expr)
{
    EvalResult res;
    try
    {
        // Build var map from BST
        ExpressionEngine::VarMap vars;
        impl_->symTable.inorder([&](const std::string &k, double v)
                                { vars[k] = v; });

        res.postfix = ExpressionEngine::infixToPostfix(expr);
        res.prefix = ExpressionEngine::infixToPrefix(expr);
        res.value = ExpressionEngine::evalPostfix(res.postfix, vars);
        res.ok = true;

        // Store in history (singly list) and undo buffer (doubly list)
        pushHistory(expr);
        impl_->undoRedo.insertLast(expr);
        impl_->undoCursor = impl_->undoRedo.tail();

        // Populate token buffer (array ops)
        auto toks = ExpressionEngine::tokenise(expr);
        impl_->tokenBuf.clear();
        for (auto &t : toks)
            impl_->tokenBuf.insertLast(t.value);

        // Record numeric result in BIT
        impl_->recordResult(res.value);
    }
    catch (const std::exception &e)
    {
        res.ok = false;
        res.error = e.what();
    }
    return res;
}

// Symbolic differentiation
DiffResult MathEngine::differentiate(const std::string &expr, const std::string &wrt)
{
    DiffResult res;
    auto sym = SymbolicEngine::diffInfix(expr, wrt);
    res.ok = sym.ok;
    res.original = sym.original;
    res.derivative = sym.derivative;
    res.error = sym.error;
    return res;
}

IntegResult MathEngine::integrate(const std::string &expr, const std::string &wrt)
{
    IntegResult res;
    auto sym = SymbolicEngine::integInfix(expr, wrt);
    res.ok = sym.ok;
    res.original = sym.original;
    res.antiderivative = sym.antiderivative;
    res.error = sym.error;
    return res;
}

// Variables 
void MathEngine::setVar(const std::string &name, double val)
{
    impl_->symTable.update(name, val);
}

bool MathEngine::getVar(const std::string &name, double &val) const
{
    return impl_->symTable.search(name, val);
}

void MathEngine::deleteVar(const std::string &name)
{
    impl_->symTable.remove(name);
}

std::vector<VariableInfo> MathEngine::listVars() const
{
    std::vector<VariableInfo> list;
    impl_->symTable.inorder([&](const std::string &k, double v)
                            { list.push_back({k, v}); });
    return list; // BST inorder is already sorted
}

// History 
void MathEngine::pushHistory(const std::string &expr)
{
    impl_->history.insertFirst(expr); // newest at front
}

std::string MathEngine::getHistory(int index) const
{
    SNode *cur = impl_->history.head();
    for (int i = 0; cur && i < index; ++i)
        cur = cur->next;
    return cur ? cur->data : "";
}

int MathEngine::historySize() const { return impl_->history.size(); }

std::string MathEngine::historyToString() const
{
    return impl_->history.toString();
}

// Undo / Redo 
bool MathEngine::canUndo() const
{
    return impl_->undoCursor && impl_->undoCursor->prev;
}
bool MathEngine::canRedo() const
{
    return impl_->undoCursor && impl_->undoCursor->next;
}

std::string MathEngine::undo()
{
    if (!canUndo())
        return "";
    impl_->undoCursor = impl_->undoCursor->prev;
    return impl_->undoCursor->data;
}

std::string MathEngine::redo()
{
    if (!canRedo())
        return "";
    impl_->undoCursor = impl_->undoCursor->next;
    return impl_->undoCursor->data;
}

// Tokenise 
std::vector<std::string> MathEngine::tokenise(const std::string &expr) const
{
    auto tokens = ExpressionEngine::tokenise(expr);
    std::vector<std::string> out;
    for (auto &t : tokens)
        out.push_back(t.value);
    return out;
}

// Sorting 
std::vector<double> MathEngine::sortValues(std::vector<double> vals, SortAlgo algo) const
{
    switch (algo)
    {
    case SortAlgo::Bubble:
        Sorting::bubbleSort(vals);
        break;
    case SortAlgo::Insertion:
        Sorting::insertionSort(vals);
        break;
    case SortAlgo::Selection:
        Sorting::selectionSort(vals);
        break;
    case SortAlgo::Quick:
        Sorting::quickSort(vals);
        break;
    case SortAlgo::Merge:
        Sorting::mergeSort(vals);
        break;
    case SortAlgo::Heap:
        Sorting::heapSort(vals);
        break;
    }
    return vals;
}

//  Graph 
void MathEngine::addDependency(const std::string &target, const std::string &dep)
{
    impl_->depGraph.addEdge(dep, target); // dep must evaluate before target
}

std::vector<std::string> MathEngine::evalOrder() const
{
    return impl_->depGraph.topoSort();
}

// BIT / prefix stats 
double MathEngine::prefixSum(int upToIndex) const
{
    if (!impl_->resultBIT || upToIndex < 1)
        return 0;
    return impl_->resultBIT->query(std::min(upToIndex, impl_->resultBIT->size()));
}

double MathEngine::rangeSum(int from, int to) const
{
    if (!impl_->resultBIT)
        return 0;
    int sz = impl_->resultBIT->size();
    return impl_->resultBIT->rangeQuery(
        std::max(1, from), std::min(to, sz));
}


// DSA Demo Methods
std::string MathEngine::demoArrayOps() const
{
    std::ostringstream o;
    ArrayOps a;
    o << "=== Array Operations Demo ===\n";
    a.insertLast("3");
    a.insertLast("1");
    a.insertLast("4");
    o << "After insertLast(3,1,4): " << a.toString() << "\n";
    a.insertFirst("0");
    o << "After insertFirst(0):    " << a.toString() << "\n";
    a.insertAt(2, "99");
    o << "After insertAt(2,99):    " << a.toString() << "\n";
    a.deleteFirst();
    o << "After deleteFirst():     " << a.toString() << "\n";
    a.deleteLast();
    o << "After deleteLast():      " << a.toString() << "\n";
    a.deleteAt(1);
    o << "After deleteAt(1):       " << a.toString() << "\n";
    return o.str();
}

std::string MathEngine::demoStackQueue() const
{
    std::ostringstream o;
    o << "=== Stack & Queue Demo ===\n";
    Stack s;
    s.push("a");
    s.push("b");
    s.push("c");
    o << "Stack after push(a,b,c): " << s.toString() << "\n";
    o << "Pop: " << s.pop() << " -> " << s.toString() << "\n";
    o << "Peek: " << s.peek() << "\n\n";

    Queue q;
    q.enqueue("x");
    q.enqueue("y");
    q.enqueue("z");
    o << "Queue after enqueue(x,y,z): " << q.toString() << "\n";
    o << "Dequeue: " << q.dequeue() << " -> " << q.toString() << "\n";
    return o.str();
}

std::string MathEngine::demoLinkedLists() const
{
    std::ostringstream o;
    o << "=== Singly & Circular Singly Linked List Demo ===\n";
    SinglyList sl;
    sl.insertLast("A");
    sl.insertLast("B");
    sl.insertLast("C");
    o << "Singly (A,B,C):          " << sl.toString() << "\n";
    sl.insertFirst("Z");
    o << "After insertFirst(Z):    " << sl.toString() << "\n";
    sl.insertAt(2, "M");
    o << "After insertAt(2,M):     " << sl.toString() << "\n";
    sl.deleteFirst();
    o << "After deleteFirst():     " << sl.toString() << "\n";
    sl.deleteVal("M");
    o << "After deleteVal(M):      " << sl.toString() << "\n\n";

    SinglyCircular sc;
    sc.insertLast("1");
    sc.insertLast("2");
    sc.insertLast("3");
    o << "Circular singly:         " << sc.toString() << "\n";
    sc.deleteFirst();
    o << "After deleteFirst():     " << sc.toString() << "\n";
    return o.str();
}

std::string MathEngine::demoDoublyLists() const
{
    std::ostringstream o;
    o << "=== Doubly & Circular Doubly Linked List Demo ===\n";
    DoublyList dl;
    dl.insertLast("X");
    dl.insertLast("Y");
    dl.insertLast("Z");
    o << "Doubly (X,Y,Z):          " << dl.toString() << "\n";
    o << "Reversed:                " << dl.toStringRev() << "\n";
    dl.insertFirst("W");
    o << "After insertFirst(W):    " << dl.toString() << "\n";
    dl.deleteLast();
    o << "After deleteLast():      " << dl.toString() << "\n\n";

    DoublyCircular dc;
    dc.insertLast("P");
    dc.insertLast("Q");
    dc.insertLast("R");
    o << "Circular doubly:         " << dc.toString() << "\n";
    dc.deleteAt(1);
    o << "After deleteAt(1):       " << dc.toString() << "\n";
    return o.str();
}

std::string MathEngine::demoSorting() const
{
    std::ostringstream o;
    o << "=== Sorting Algorithms Demo ===\n";
    std::vector<double> v = {5, 3, 8, 1, 9, 2, 7, 4, 6};
    o << "Original:   " << Sorting::toString(v) << "\n";

    auto run = [&](const std::string &name, auto fn)
    {
        std::vector<double> tmp = v;
        fn(tmp);
        o << std::left << std::setw(14) << name << Sorting::toString(tmp) << "\n";
    };
    run("Bubble:", [](auto &x)
        { Sorting::bubbleSort(x); });
    run("Insertion:", [](auto &x)
        { Sorting::insertionSort(x); });
    run("Selection:", [](auto &x)
        { Sorting::selectionSort(x); });
    run("Quick:", [](auto &x)
        { Sorting::quickSort(x); });
    run("Merge:", [](auto &x)
        { Sorting::mergeSort(x); });
    run("Heap:", [](auto &x)
        { HeapSort::sort(x); });
    return o.str();
}

std::string MathEngine::demoBST() const
{
    std::ostringstream o;
    o << "=== BST Demo ===\n";
    BST b;
    b.insert("pi", 3.14159);
    b.insert("e", 2.71828);
    b.insert("phi", 1.61803);
    b.insert("sqrt2", 1.41421);
    o << "Inorder:  " << b.toStringInorder() << "\n";
    double v;
    b.search("e", v);
    o << "Search e: " << v << "\n";
    b.remove("pi");
    o << "After delete pi: " << b.toStringInorder() << "\n";
    o << "Min: " << (b.minNode() ? b.minNode()->key : "none") << "\n";
    o << "Max: " << (b.maxNode() ? b.maxNode()->key : "none") << "\n";
    return o.str();
}

std::string MathEngine::demoBIT() const
{
    std::ostringstream o;
    o << "=== Binary Indexed Tree (Fenwick) Demo ===\n";
    std::vector<double> data = {3, 2, -1, 6, 5, 4, -3, 3, 7, 2};
    BIT bit(data);
    o << "Array: ";
    for (auto x : data)
        o << x << " ";
    o << "\n";
    o << "prefix_sum(5)  = " << bit.query(5) << "  (expect 15)\n";
    o << "range_sum(3,7) = " << bit.rangeQuery(3, 7) << "  (expect 11)\n";
    bit.update(3, 4);
    o << "After update(3,+4), prefix_sum(5) = " << bit.query(5) << "\n";
    return o.str();
}

std::string MathEngine::demoHeap() const
{
    std::ostringstream o;
    o << "=== Heap Demo ===\n";
    std::vector<double> data = {5, 3, 8, 1, 9, 2};
    MinHeap mn(data);
    MaxHeap mx(data);
    o << mn.toString() << "\n";
    o << mx.toString() << "\n";
    o << "MinHeap pop sequence: ";
    while (!mn.empty())
        o << mn.pop() << " ";
    o << "\nMaxHeap pop sequence: ";
    while (!mx.empty())
        o << mx.pop() << " ";
    o << "\n";

    std::vector<double> hs = data;
    HeapSort::sort(hs);
    o << "HeapSort result: " << Sorting::toString(hs) << "\n";
    return o.str();
}

std::string MathEngine::demoGraph() const
{
    std::ostringstream o;
    o << "=== Graph: BFS, DFS, Topological Sort Demo ===\n";
    Graph g;
    // Dependency: a->b, a->c, b->d, c->d, d->e
    g.addEdge("a", "b");
    g.addEdge("a", "c");
    g.addEdge("b", "d");
    g.addEdge("c", "d");
    g.addEdge("d", "e");
    o << "Adjacency:\n"
      << g.toString();

    std::ostringstream bfsO, dfsO;
    g.bfs("a", [&](const std::string &n)
          { bfsO << n << " "; });
    g.dfs("a", [&](const std::string &n)
          { dfsO << n << " "; });
    o << "BFS from a:       " << bfsO.str() << "\n";
    o << "DFS from a:       " << dfsO.str() << "\n";

    auto topo = g.topoSort();
    o << "Topological sort: ";
    for (auto &n : topo)
        o << n << " ";
    o << "\n";
    return o.str();
}
