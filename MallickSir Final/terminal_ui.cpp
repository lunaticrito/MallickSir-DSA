// terminal_ui.cpp 

#include "math_engine.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

static void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void pause()
{
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static void printBanner()
{
    std::cout << R"(
============================================================
|           M  A  L  L  I  C  K  S  I  R                   |
|              M A T H   E N G I N E                       |
============================================================
)" << "\n";
}

static void printMainMenu()
{
    std::cout << "-----------------------------------------\n"
              << "|  MAIN MENU                              |\n"
              << "-----------------------------------------\n"
              << "|  1. Evaluate expression                 |\n"
              << "|  2. Variables                           |\n"
              << "|  3. Expression history                  |\n"
              << "|  4. Undo / Redo                         |\n"
              << "|  5. Sort values                         |\n"
              << "|  6. BIT prefix/range sum                |\n"
              << "|  7. Graph dependency order              |\n"
              << "|  8. Symbolic differentiate (CAS)        |\n"
              << "|  9. Symbolic integrate (CAS)            |\n"
              << "|-----------------------------------------|\n"
              /*/ << "|  10. DSA Demo: Arrays                   |\n"
               << "|  11. DSA Demo: Stack & Queue            |\n"
               << "|  12. DSA Demo: Singly Linked Lists      |\n"
               << "|  13. DSA Demo: Doubly Linked Lists      |\n"
               << "|  14. DSA Demo: Sorting                  |\n"
               << "|  15. DSA Demo: BST                      |\n"
               << "|  16. DSA Demo: BIT (Fenwick Tree)       |\n"
               << "|  17. DSA Demo: Heap                     |\n"
               << "|  18. DSA Demo: Graph (BFS/DFS/Topo)     |\n"
               << "|-----------------------------------------|\n"
               << "|  0. Exit                                |\n"
               << "+-----------------------------------------+\n"*/
              << "Choice: ";
}

// Sub-menus 
static void menuEval(MathEngine &eng)
{
    std::cout << "\nExpression (supports +,-,*,/,^,%,sin,cos,sqrt,log,...)\n"
              << "Example: 3 + sin(pi/2) * sqrt(4)\n"
              << "Enter: ";
    std::string expr;
    std::getline(std::cin, expr);
    if (expr.empty())
        return;

    auto r = eng.evaluate(expr);
    if (!r.ok)
    {
        std::cout << "  Error: " << r.error << "\n";
    }
    else
    {
        std::cout << std::fixed << std::setprecision(8);
        std::cout << "  Result  : " << r.value << "\n";
        std::cout << "  Postfix : " << r.postfix << "\n";
        std::cout << "  Prefix  : " << r.prefix << "\n";

        // Show token buffer
        auto toks = eng.tokenise(expr);
        std::cout << "  Tokens  : [";
        for (int i = 0; i < (int)toks.size(); ++i)
        {
            if (i)
                std::cout << ", ";
            std::cout << toks[i];
        }
        std::cout << "]\n";
    }
    pause();
}

static void menuDiff(MathEngine &eng)
{
    std::cout << "\n-- Symbolic Differentiation (CAS) ---------\n"
              << "  Supports: + - * / ^ sin cos log ln exp sqrt\n"
              << "  Example: x^3 + sin(x)   differentiate w.r.t. x\n"
              << "  Expression: ";
    std::string expr;
    std::getline(std::cin, expr);
    if (expr.empty())
        return;

    std::cout << "  Differentiate with respect to: ";
    std::string wrt;
    std::getline(std::cin, wrt);
    if (wrt.empty())
        wrt = "x";

    auto r = eng.differentiate(expr, wrt);
    if (!r.ok)
    {
        std::cout << "  Error: " << r.error << "\n";
    }
    else
    {
        std::cout << "  f(" << wrt << ") = " << r.original << "\n";
        std::cout << "  d/d" << wrt << " = " << r.derivative << "\n";
    }
    pause();
}

static void menuInteg(MathEngine &eng)
{
    std::cout << "\n-- Symbolic Integration (CAS) -------------\n"
              << "  Supports: + - * / ^ sin cos log ln exp sqrt\n"
              << "  Example: x^3 + sin(x)   integrate w.r.t. x\n"
              << "  Expression: ";
    std::string expr;
    std::getline(std::cin, expr);
    if (expr.empty())
        return;

    std::cout << "  Integrate with respect to: ";
    std::string wrt;
    std::getline(std::cin, wrt);
    if (wrt.empty())
        wrt = "x";

    auto r = eng.integrate(expr, wrt);
    if (!r.ok)
    {
        std::cout << "  Error: " << r.error << "\n";
    }
    else
    {
        std::cout << "  f(" << wrt << ") = " << r.original << "\n";
        std::cout << "  int d" << wrt << " = " << r.antiderivative << "\n";
    }
    pause();
}

static void menuVars(MathEngine &eng)
{
    while (true)
    {
        std::cout << "\n-- Variables ---------------------------------\n";
        auto vars = eng.listVars();
        if (vars.empty())
            std::cout << "  (no variables)\n";
        else
            for (auto &v : vars)
                std::cout << "  " << std::left << std::setw(12) << v.name
                          << " = " << v.value << "\n";
        std::cout << "\n  [s]et  [d]elete  [b]ack: ";
        std::string cmd;
        std::getline(std::cin, cmd);
        if (cmd == "s")
        {
            std::cout << "  Name: ";
            std::string name;
            std::getline(std::cin, name);
            std::cout << "  Value: ";
            std::string sval;
            std::getline(std::cin, sval);
            try
            {
                eng.setVar(name, std::stod(sval));
                std::cout << "  Set.\n";
            }
            catch (...)
            {
                std::cout << "  Invalid number.\n";
            }
        }
        else if (cmd == "d")
        {
            std::cout << "  Name: ";
            std::string name;
            std::getline(std::cin, name);
            eng.deleteVar(name);
            std::cout << "  Deleted (if existed).\n";
        }
        else
            break;
    }
}

static void menuHistory(MathEngine &eng)
{
    std::cout << "\n-- Expression History (newest first) -------\n";
    if (eng.historySize() == 0)
        std::cout << "  (empty)\n";
    else
    {
        for (int i = 0; i < eng.historySize(); ++i)
            std::cout << "  [" << i << "] " << eng.getHistory(i) << "\n";
    }
    pause();
}

static void menuUndoRedo(MathEngine &eng)
{
    std::cout << "\n-- Undo / Redo ------------------------------\n";
    std::cout << "  [u]ndo  [r]edo  [b]ack: ";
    std::string cmd;
    std::getline(std::cin, cmd);
    if (cmd == "u")
    {
        if (eng.canUndo())
            std::cout << "  Undid to: " << eng.undo() << "\n";
        else
            std::cout << "  Nothing to undo.\n";
    }
    else if (cmd == "r")
    {
        if (eng.canRedo())
            std::cout << "  Redid to: " << eng.redo() << "\n";
        else
            std::cout << "  Nothing to redo.\n";
    }
    pause();
}

static void menuSort(MathEngine &eng)
{
    std::cout << "\n-- Sort Values ------------------------------\n"
              << "  Enter numbers separated by spaces: ";
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    std::vector<double> vals;
    double x;
    while (iss >> x)
        vals.push_back(x);
    if (vals.empty())
    {
        std::cout << "  No values entered.\n";
        pause();
        return;
    }

    std::cout << "  Algorithm: [1]Bubble [2]Insertion [3]Selection "
              << "[4]Quick [5]Merge [6]Heap: ";
    std::string ch;
    std::getline(std::cin, ch);
    MathEngine::SortAlgo algo = MathEngine::SortAlgo::Quick;
    if (ch == "1")
        algo = MathEngine::SortAlgo::Bubble;
    else if (ch == "2")
        algo = MathEngine::SortAlgo::Insertion;
    else if (ch == "3")
        algo = MathEngine::SortAlgo::Selection;
    else if (ch == "4")
        algo = MathEngine::SortAlgo::Quick;
    else if (ch == "5")
        algo = MathEngine::SortAlgo::Merge;
    else if (ch == "6")
        algo = MathEngine::SortAlgo::Heap;

    auto sorted = eng.sortValues(vals, algo);
    std::cout << "  Sorted: ";
    for (auto v : sorted)
        std::cout << v << " ";
    std::cout << "\n";
    pause();
}

static void menuBIT(MathEngine &eng)
{
    std::cout << "\n-- BIT Prefix / Range Sum -------------------\n";
    int n = eng.historySize();
    if (n == 0)
    {
        std::cout << "  Evaluate some expressions first to populate the BIT.\n";
        pause();
        return;
    }
    std::cout << "  " << n << " results recorded.\n";
    std::cout << "  Prefix sum up to index (1-" << n << "): ";
    std::string s;
    std::getline(std::cin, s);
    try
    {
        int idx = std::stoi(s);
        std::cout << "  prefix_sum(1.." << idx << ") = "
                  << eng.prefixSum(idx) << "\n";
    }
    catch (...)
    {
    }
    std::cout << "  Range sum [from to] (e.g. 2 5): ";
    std::getline(std::cin, s);
    std::istringstream iss(s);
    int a, b;
    iss >> a >> b;
    std::cout << "  range_sum(" << a << ".." << b << ") = "
              << eng.rangeSum(a, b) << "\n";
    pause();
}

static void menuGraph(MathEngine &eng)
{
    std::cout << "\n-- Expression Dependency Graph --------------\n"
              << "  Add edge: 'target depends_on dep'  (e.g. y x)\n"
              << "  Type 'go' to compute eval order, 'back' to exit\n";
    while (true)
    {
        std::cout << "  > ";
        std::string line;
        std::getline(std::cin, line);
        if (line == "back")
            break;
        if (line == "go")
        {
            auto order = eng.evalOrder();
            if (order.empty())
                std::cout << "  Cycle detected!\n";
            else
            {
                std::cout << "  Evaluation order: ";
                for (auto &n : order)
                    std::cout << n << " ";
                std::cout << "\n";
            }
        }
        else
        {
            std::istringstream iss(line);
            std::string t, d;
            iss >> t >> d;
            if (!t.empty() && !d.empty())
            {
                eng.addDependency(t, d);
                std::cout << "  Added: " << t << " depends on " << d << "\n";
            }
        }
    }
}

int main()
{
    MathEngine eng;

    //some useful constants
    eng.setVar("pi", 3.14159265358979);
    eng.setVar("e", 2.71828182845905);
    eng.setVar("phi", 1.61803398874989);
    eng.setVar("sqrt2", 1.41421356237310);

    clearScreen();
    printBanner();
    std::cout << "  Constants loaded: pi, e, phi, sqrt2\n\n";

    while (true)
    {
        printMainMenu();
        std::string choice;
        std::getline(std::cin, choice);

        clearScreen();
        printBanner();

        int c = 0;
        try
        {
            c = std::stoi(choice);
        }
        catch (...)
        {
            continue;
        }

        switch (c)
        {
        case 0:
            std::cout << "  Goodbye!\n";
            return 0;
        case 1:
            menuEval(eng);
            break;
        case 2:
            menuVars(eng);
            break;
        case 3:
            menuHistory(eng);
            break;
        case 4:
            menuUndoRedo(eng);
            break;
        case 5:
            menuSort(eng);
            break;
        case 6:
            menuBIT(eng);
            break;
        case 7:
            menuGraph(eng);
            break;
        case 8:
            menuDiff(eng);
            break;
        case 9:
            menuInteg(eng);
            break;
        case 10:
            std::cout << eng.demoArrayOps();
            pause();
            break;
        case 11:
            std::cout << eng.demoStackQueue();
            pause();
            break;
        case 12:
            std::cout << eng.demoLinkedLists();
            pause();
            break;
        case 13:
            std::cout << eng.demoDoublyLists();
            pause();
            break;
        case 14:
            std::cout << eng.demoSorting();
            pause();
            break;
        case 15:
            std::cout << eng.demoBST();
            pause();
            break;
        case 16:
            std::cout << eng.demoBIT();
            pause();
            break;
        case 17:
            std::cout << eng.demoHeap();
            pause();
            break;
        case 18:
            std::cout << eng.demoGraph();
            pause();
            break;
        default:
            std::cout << "  Unknown option.\n";
            pause();
            break;
        }
        clearScreen();
        printBanner();
    }
}
