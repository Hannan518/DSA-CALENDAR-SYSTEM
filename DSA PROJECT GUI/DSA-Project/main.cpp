#include "mainwindow.h"
#include <QApplication>

// Defines the modern look and feel (CSS)
const QString STYLESHEET = R"(
    QMainWindow { background-color: #f0f2f5; }
    QFrame#SidePanel { background-color: #154c79; border-radius: 15px; }
    QFrame#CenterPanel { background-color: white; border-radius: 15px; }
    QLabel { font-family: 'Segoe UI', sans-serif; }
    QLabel#SideLabel { color: white; font-weight: bold; }
    QLabel#HeaderLabel { color: #154c79; font-weight: 900; letter-spacing: 1px; }
    QPushButton#MonthBtn { text-align: left; padding: 12px 20px; border: none; color: #a0c4d9; font-size: 15px; background-color: transparent; }
    QPushButton#MonthBtn:checked { color: white; font-weight: bold; border-left: 5px solid #feca57; background-color: rgba(0, 0, 0, 0.1); }
    QPushButton#MonthBtn:hover { color: white; }
    QPushButton#ActionBtn { background-color: white; color: #154c79; border-radius: 8px; padding: 12px; font-weight: bold; font-size: 13px; border: none; }
    QPushButton#ActionBtn:hover { background-color: #ecf0f1; }
    QPushButton#UpcomingBtn { background-color: #feca57; color: #2d3436; border-radius: 8px; padding: 12px; font-weight: bold; border: none; }
    QPushButton#UpcomingBtn:hover { background-color: #ff9f43; }
    QLineEdit#SearchBar { padding: 10px 15px; border: 2px solid #dfe6e9; border-radius: 20px; background-color: #f8f9fa; color: #2d3436; font-size: 14px; }
    QLineEdit#SearchBar:focus { border: 2px solid #154c79; background-color: white; }
)";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(STYLESHEET);
    MainWindow w;
    w.show();
    return a.exec();
}
