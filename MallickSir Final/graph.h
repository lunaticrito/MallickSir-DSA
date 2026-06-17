#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>


// Graph: BFS, DFS, Topological Sort
// a dependency graph to order multi-expression evaluation
// Nodes are named (string), edges are directed (dependency)


class Graph {
public:
    // Construction 
    void addNode(const std::string& name);
    void addEdge(const std::string& from, const std::string& to);
    bool hasNode(const std::string& name) const;
    bool hasEdge(const std::string& from, const std::string& to) const;
    int  nodeCount() const;
    int  edgeCount() const;

    // Traversals 
    using Visitor = std::function<void(const std::string&)>;
    void bfs(const std::string& start, Visitor fn) const;
    void dfs(const std::string& start, Visitor fn) const;

    // Topological Sort (returns sorted order) 
    // Returns empty vector if cycle detected
    std::vector<std::string> topoSort() const;

    // Utility
    std::string toString()  const;
    void        clear();
    std::vector<std::string> neighbors(const std::string& node) const;
    std::vector<std::string> allNodes() const;

private:
    std::unordered_map<std::string, std::vector<std::string>> adj_;
    int edgeCount_ = 0;

    void dfsHelper(const std::string& node,
                   std::unordered_map<std::string, bool>& visited,
                   Visitor& fn) const;
};
