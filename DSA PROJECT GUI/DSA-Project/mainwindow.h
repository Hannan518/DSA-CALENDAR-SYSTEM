#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QLineEdit>
#include <vector>
#include "calendar.h"
#include "eventcalendar.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDateSelected();
    void onMonthBtnClicked(int monthIndex);
    void onAddEventClicked();
    void onDeleteEventClicked();
    void onEditEventClicked();

    // RESTORED: Upcoming (Next 7 Days)
    void onUpcomingClicked();

    // KEEPING: View All (Everything)
    void onViewAllClicked();

    // KEEPING: Clear All
    void onClearAllClicked();

    void onSearchTextChanged(const QString &text);

private:
    void setupUI();
    void loadData();
    void saveData();
    void refreshEventList(const std::string& dateStr);
    void updateMonthButtons(int currentMonth);

    void searchRecursive(BSTNode* node, const std::string& query, std::vector<Event>& results);

    CalendarSystem calendarSystem;

    QFrame *leftPanel;
    QFrame *centerPanel;
    QFrame *rightPanel;
    QLabel *headerLabel;
    QLabel *dateDetailLabel;
    QLineEdit *searchBar;
    EventCalendar *calendar;
    QListWidget *eventList;
    std::vector<QPushButton*> monthButtons;
};

#endif
