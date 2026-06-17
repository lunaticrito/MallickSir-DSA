#pragma once
#include <vector>
#include <string>

// ─────────────────────────────────────────────────────────────────────────────
// DSA Module 1 — Array Operations
// Insert / Delete at first, last, and middle positions.
// Used by MathEngine as a dynamic token buffer.
// ─────────────────────────────────────────────────────────────────────────────

class ArrayOps {
public:
    // ── Insert ──────────────────────────────────────────────────────────────
    void insertFirst(const std::string& val);
    void insertLast(const std::string& val);
    void insertAt(int index, const std::string& val);   // 0-based

    // ── Delete ──────────────────────────────────────────────────────────────
    bool deleteFirst();
    bool deleteLast();
    bool deleteAt(int index);                           // 0-based

    // ── Access ──────────────────────────────────────────────────────────────
    const std::string& get(int index) const;
    int  size()  const;
    bool empty() const;
    void clear();

    // ── Utility ─────────────────────────────────────────────────────────────
    std::string toString() const;
    const std::vector<std::string>& data() const { return arr_; }

private:
    std::vector<std::string> arr_;
};
