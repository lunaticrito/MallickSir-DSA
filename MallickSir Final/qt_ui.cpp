
// for qt Graphics UI (Qt6 stub)
// no more needed
/*
#include "math_engine.h"
#include <QApplication>
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QTabWidget>
#include <QString>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("MathEngine");
        setMinimumSize(800, 600);

        auto* central = new QWidget(this);
        auto* root    = new QVBoxLayout(central);

        // ── Expression bar ──────────────────────────────────────────────────
        auto* bar   = new QHBoxLayout();
        exprInput_  = new QLineEdit();
        auto* btn   = new QPushButton("Evaluate");
        bar->addWidget(exprInput_);
        bar->addWidget(btn);
        root->addLayout(bar);

        resultLabel_ = new QLabel("Result: ");
        root->addWidget(resultLabel_);

        // ── Tabs ─────────────────────────────────────────────────────────────
        auto* tabs = new QTabWidget();
        // Add tabs: History, Variables, DSA Demos ...
        root->addWidget(tabs);

        setCentralWidget(central);

        connect(btn, &QPushButton::clicked, this, &MainWindow::onEval);
        connect(exprInput_, &QLineEdit::returnPressed, this, &MainWindow::onEval);
    }

private slots:
    void onEval() {
        std::string expr = exprInput_->text().toStdString();
        auto r = engine_.evaluate(expr);
        if (r.ok)
            resultLabel_->setText(QString("Result: %1   Postfix: %2")
                .arg(r.value).arg(QString::fromStdString(r.postfix)));
        else
            resultLabel_->setText(QString("Error: %1")
                .arg(QString::fromStdString(r.error)));
    }

private:
    MathEngine engine_;
    QLineEdit* exprInput_  = nullptr;
    QLabel*    resultLabel_ = nullptr;
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

#include "qt_ui.moc"
*/

// This file intentionally empty for Phase 1.
// Uncomment the block above and enable Qt in CMakeLists.txt for Phase 2.
int unused_qt_placeholder = 0;
