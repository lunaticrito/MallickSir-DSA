#include "graph.h"
#include <queue>
#include <stack>
#include <sstream>
#include <algorithm>

void Graph::addNode(const std::string& name) {
    if (adj_.find(name) == adj_.end())
        adj_[name] = {};
}

void Graph::addEdge(const std::string& from, const std::string& to) {
    addNode(from);
    addNode(to);
    auto& nbrs = adj_[from];
    if (std::find(nbrs.begin(), nbrs.end(), to) == nbrs.end()) {
        nbrs.push_back(to);
        ++edgeCount_;
    }
}

bool Graph::hasNode(const std::string& name) const {
    return adj_.find(name) != adj_.end();
}

bool Graph::hasEdge(const std::string& from, const std::string& to) const {
    auto it = adj_.find(from);
    if (it == adj_.end()) return false;
    return std::find(it->second.begin(), it->second.end(), to) != it->second.end();
}

int Graph::nodeCount() const { return (int)adj_.size(); }
int Graph::edgeCount() const { return edgeCount_; }

std::vector<std::string> Graph::neighbors(const std::string& node) const {
    auto it = adj_.find(node);
    if (it == adj_.end()) return {};
    return it->second;
}

std::vector<std::string> Graph::allNodes() const {
    std::vector<std::string> nodes;
    for (auto& [k, _] : adj_) nodes.push_back(k);
    std::sort(nodes.begin(), nodes.end());
    return nodes;
}

// BFS 
void Graph::bfs(const std::string& start, Visitor fn) const {
    if (!hasNode(start)) return;
    std::unordered_map<std::string, bool> visited;
    std::queue<std::string> q;
    q.push(start);
    visited[start] = true;
    while (!q.empty()) {
        auto node = q.front(); q.pop();
        fn(node);
        for (auto& nb : adj_.at(node)) {
            if (!visited[nb]) {
                visited[nb] = true;
                q.push(nb);
            }
        }
    }
}

// DFS 
void Graph::dfsHelper(const std::string& node,
                      std::unordered_map<std::string, bool>& visited,
                      Visitor& fn) const {
    visited[node] = true;
    fn(node);
    for (auto& nb : adj_.at(node))
        if (!visited[nb]) dfsHelper(nb, visited, fn);
}

void Graph::dfs(const std::string& start, Visitor fn) const {
    if (!hasNode(start)) return;
    std::unordered_map<std::string, bool> visited;
    dfsHelper(start, visited, fn);
}

// Topological Sort
std::vector<std::string> Graph::topoSort() const {
    std::unordered_map<std::string, int> inDeg;
    for (auto& [n, _] : adj_) inDeg[n] = 0;
    for (auto& [n, nbrs] : adj_)
        for (auto& nb : nbrs) ++inDeg[nb];

    std::queue<std::string> q;
    for (auto& [n, d] : inDeg) if (d == 0) q.push(n);

    std::vector<std::string> order;
    while (!q.empty()) {
        auto node = q.front(); q.pop();
        order.push_back(node);
        for (auto& nb : adj_.at(node))
            if (--inDeg[nb] == 0) q.push(nb);
    }
    if ((int)order.size() != (int)adj_.size()) return {};  // cycle
    return order;
}

std::string Graph::toString() const {
    std::ostringstream oss;
    for (auto& node : allNodes()) {
        oss << node << " → [";
        auto& nbrs = adj_.at(node);
        for (int i = 0; i < (int)nbrs.size(); ++i) {
            if (i) oss << ", ";
            oss << nbrs[i];
        }
        oss << "]\n";
    }
    return oss.str();
}

void Graph::clear() {
    adj_.clear();
    edgeCount_ = 0;
}
