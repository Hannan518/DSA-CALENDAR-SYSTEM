#include "mainwindow.h"
#include "addeventdialog.h"
#include "file_manager.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGraphicsDropShadowEffect>
#include <QDate>
#include <QMessageBox>
#include <QLocale>
#include <QApplication>
#include <QStyle>
#include <functional>
#include <algorithm> // Added for std::sort

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("DSA Calendar System");
    resize(1280, 800);
    setupUI();
    loadData();

    calendar->setSelectedDate(QDate::currentDate());
    onDateSelected();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(25); mainLayout->setContentsMargins(25, 25, 25, 25);

    // --- LEFT PANEL ---
    leftPanel = new QFrame(); leftPanel->setObjectName("SidePanel"); leftPanel->setFixedWidth(240);
    QGraphicsDropShadowEffect *shadow1 = new QGraphicsDropShadowEffect();
    shadow1->setBlurRadius(20); shadow1->setOffset(0,8); shadow1->setColor(QColor(0,0,0,50));
    leftPanel->setGraphicsEffect(shadow1);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 30, 0, 30); leftLayout->setSpacing(5);
    QLabel *yearLabel = new QLabel("2025"); yearLabel->setObjectName("SideLabel");
    yearLabel->setAlignment(Qt::AlignCenter); yearLabel->setStyleSheet("font-size: 36px; margin-bottom: 20px; font-weight: 900;");
    yearLabel->setMinimumHeight(60); leftLayout->addWidget(yearLabel);
    QStringList months = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    for(int i = 0; i < 12; i++) {
        QPushButton *btn = new QPushButton(months[i]); btn->setObjectName("MonthBtn");
        btn->setCheckable(true); btn->setCursor(Qt::PointingHandCursor);
        connect(btn, &QPushButton::clicked, [this, i]() { onMonthBtnClicked(i + 1); });
        leftLayout->addWidget(btn); monthButtons.push_back(btn);
    }
    leftLayout->addStretch(); mainLayout->addWidget(leftPanel);

    // --- CENTER PANEL ---
    centerPanel = new QFrame(); centerPanel->setObjectName("CenterPanel");
    QGraphicsDropShadowEffect *shadow2 = new QGraphicsDropShadowEffect();
    shadow2->setBlurRadius(20); shadow2->setOffset(0,8); shadow2->setColor(QColor(0,0,0,30));
    centerPanel->setGraphicsEffect(shadow2);
    QVBoxLayout *centerLayout = new QVBoxLayout(centerPanel);
    centerLayout->setSpacing(15); centerLayout->setContentsMargins(40, 40, 40, 40);

    // SEARCH BAR
    searchBar = new QLineEdit(); searchBar->setObjectName("SearchBar");
    searchBar->setPlaceholderText("Search events (Title or ID)...");
    connect(searchBar, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    centerLayout->addWidget(searchBar);

    headerLabel = new QLabel("DECEMBER"); headerLabel->setObjectName("HeaderLabel");
    headerLabel->setAlignment(Qt::AlignCenter); headerLabel->setStyleSheet("font-size: 28px; margin-top: 10px; margin-bottom: 10px;");
    centerLayout->addWidget(headerLabel);
    QHBoxLayout *daysLayout = new QHBoxLayout();
    QStringList days = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
    for(const QString &day : days) {
        QLabel *dayLbl = new QLabel(day); dayLbl->setAlignment(Qt::AlignCenter);
        QString color = (day == "SUN" || day == "SAT") ? "#e74c3c" : "#7f8c8d";
        dayLbl->setStyleSheet("color: " + color + "; font-weight: bold; font-size: 13px; padding-bottom: 5px;");
        daysLayout->addWidget(dayLbl);
    }
    centerLayout->addLayout(daysLayout);
    calendar = new EventCalendar(); calendar->setGridVisible(false);
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    calendar->setHorizontalHeaderFormat(QCalendarWidget::NoHorizontalHeader);
    calendar->setNavigationBarVisible(false);
    calendar->setStyleSheet("QCalendarWidget QWidget { background-color: white; } QCalendarWidget QAbstractItemView { font-size: 18px; color: #2d3436; selection-background-color: #154c79; selection-color: white; outline: none; }");
    connect(calendar, &QCalendarWidget::selectionChanged, this, &MainWindow::onDateSelected);
    centerLayout->addWidget(calendar); mainLayout->addWidget(centerPanel, 2);

    // --- RIGHT PANEL ---
    rightPanel = new QFrame(); rightPanel->setObjectName("SidePanel"); rightPanel->setFixedWidth(320);
    QGraphicsDropShadowEffect *shadow3 = new QGraphicsDropShadowEffect();
    shadow3->setBlurRadius(20); shadow3->setOffset(0,8); shadow3->setColor(QColor(0,0,0,50));
    rightPanel->setGraphicsEffect(shadow3);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(25, 35, 25, 35); rightLayout->setSpacing(20);
    dateDetailLabel = new QLabel("DATE"); dateDetailLabel->setObjectName("SideLabel");
    dateDetailLabel->setStyleSheet("font-size: 22px; border-bottom: 1px solid rgba(255,255,255,0.3); padding-bottom: 15px;");
    dateDetailLabel->setWordWrap(true); rightLayout->addWidget(dateDetailLabel);
    eventList = new QListWidget();
    eventList->setStyleSheet("background: transparent; border: none; color: white; font-size: 14px;");
    rightLayout->addWidget(eventList);

    QGridLayout *btnGrid = new QGridLayout(); btnGrid->setSpacing(15);
    QPushButton *btnAdd = new QPushButton("ADD"); btnAdd->setObjectName("ActionBtn");
    btnAdd->setCursor(Qt::PointingHandCursor); connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddEventClicked);
    QPushButton *btnEdit = new QPushButton("EDIT"); btnEdit->setObjectName("ActionBtn");
    btnEdit->setCursor(Qt::PointingHandCursor); connect(btnEdit, &QPushButton::clicked, this, &MainWindow::onEditEventClicked);
    QPushButton *btnDel = new QPushButton("REMOVE"); btnDel->setObjectName("ActionBtn");
    btnDel->setCursor(Qt::PointingHandCursor); connect(btnDel, &QPushButton::clicked, this, &MainWindow::onDeleteEventClicked);

    QPushButton *btnUpcoming = new QPushButton("UPCOMING (30 DAYS)"); btnUpcoming->setObjectName("UpcomingBtn");
    btnUpcoming->setStyleSheet("background-color: #feca57; color: #2d3436; border: none; border-radius: 8px; font-weight: bold; padding: 10px;");
    btnUpcoming->setCursor(Qt::PointingHandCursor); connect(btnUpcoming, &QPushButton::clicked, this, &MainWindow::onUpcomingClicked);

    QPushButton *btnAll = new QPushButton("VIEW ALL EVENTS"); btnAll->setObjectName("UpcomingBtn");
    btnAll->setStyleSheet("background-color: #3498db; color: white; border: none; border-radius: 8px; font-weight: bold; padding: 10px;");
    btnAll->setCursor(Qt::PointingHandCursor); connect(btnAll, &QPushButton::clicked, this, &MainWindow::onViewAllClicked);

    QPushButton *btnClear = new QPushButton("CLEAR ALL DATA");
    btnClear->setObjectName("ActionBtn");
    btnClear->setStyleSheet("background-color: #c0392b; color: white; border: none; border-radius: 8px; font-weight: bold; padding: 10px;");
    btnClear->setCursor(Qt::PointingHandCursor);
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::onClearAllClicked);

    // Add buttons to grid
    btnGrid->addWidget(btnAdd, 0, 0); btnGrid->addWidget(btnEdit, 0, 1);
    btnGrid->addWidget(btnDel, 1, 0, 1, 2);
    btnGrid->addWidget(btnUpcoming, 2, 0, 1, 2);
    btnGrid->addWidget(btnAll, 3, 0, 1, 2);
    btnGrid->addWidget(btnClear, 4, 0, 1, 2);

    rightLayout->addLayout(btnGrid); mainLayout->addWidget(rightPanel);
}

void MainWindow::loadData() {
    loadEventsFromFile("calendar_data.txt", calendarSystem);
}
void MainWindow::saveData() { saveEventsToFile("calendar_data.txt", calendarSystem.getBST()); }

void MainWindow::onClearAllClicked() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Clear All Data",
                                  "Are you sure you want to permanently delete ALL events?\nThis action cannot be undone.",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        calendarSystem.clearAllEvents();
        saveData();
        onDateSelected();
        QMessageBox::information(this, "Success", "All events have been cleared from memory and disk.");
    }
}

// --- UPCOMING (30 DAYS) ---
void MainWindow::onUpcomingClicked() {
    searchBar->clear();
    dateDetailLabel->setText("NEXT 30 DAYS");
    eventList->clear();

    QDate today = QDate::currentDate();
    QDate limit = today.addDays(30);
    std::vector<Event> upcoming;

    std::function<void(BSTNode*)> collectUpcoming = [&](BSTNode* node) {
        if (!node) return;

        QDate nodeDate = QDate::fromString(QString::fromStdString(node->date), "d:M:yyyy");
        if (!nodeDate.isValid()) {
            nodeDate = QDate::fromString(QString::fromStdString(node->date), "dd:MM:yyyy");
        }

        if (nodeDate.isValid() && nodeDate >= today && nodeDate <= limit) {
            ListNode* curr = node->events.head;
            while(curr) {
                upcoming.push_back(curr->data);
                curr = curr->next;
            }
        }
        collectUpcoming(node->left);
        collectUpcoming(node->right);
    };
    collectUpcoming(calendarSystem.getBST()->root);

    if(upcoming.empty()) { eventList->addItem(new QListWidgetItem("No events in next 30 days")); return; }

    // SORTING FIX: Sort events by Date and Time before displaying
    std::sort(upcoming.begin(), upcoming.end(), [](const Event& a, const Event& b) {
        QDate dateA = QDate::fromString(QString::fromStdString(a.date), "d:M:yyyy");
        if(!dateA.isValid()) dateA = QDate::fromString(QString::fromStdString(a.date), "dd:MM:yyyy");

        QDate dateB = QDate::fromString(QString::fromStdString(b.date), "d:M:yyyy");
        if(!dateB.isValid()) dateB = QDate::fromString(QString::fromStdString(b.date), "dd:MM:yyyy");

        if (dateA != dateB) return dateA < dateB;
        return a.startTime < b.startTime;
    });

    for (const auto& e : upcoming) {
        QListWidgetItem *item = new QListWidgetItem(); QWidget *container = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(container); layout->setContentsMargins(0, 5, 0, 5);
        QLabel *dot = new QLabel("●"); dot->setStyleSheet("color: #feca57; font-size: 18px;");
        QVBoxLayout *infoLayout = new QVBoxLayout();
        QLabel *title = new QLabel(QString::fromStdString(e.title)); title->setStyleSheet("font-weight: bold; font-size: 14px; color: white;");
        QLabel *time = new QLabel(QString::fromStdString(e.date + " | " + e.startTime)); time->setStyleSheet("color: #ccc; font-size: 12px;");
        infoLayout->addWidget(title); infoLayout->addWidget(time); layout->addWidget(dot); layout->addLayout(infoLayout); layout->addStretch();
        item->setSizeHint(container->sizeHint()); item->setData(Qt::UserRole, QString::fromStdString(e.id));
        eventList->addItem(item); eventList->setItemWidget(item, container);
    }
}

// --- VIEW ALL EVENTS ---
void MainWindow::onViewAllClicked() {
    searchBar->clear();
    dateDetailLabel->setText("ALL EVENTS");
    eventList->clear();

    std::vector<Event> allEvents;
    std::function<void(BSTNode*)> collectAll = [&](BSTNode* node) {
        if (!node) return;
        ListNode* curr = node->events.head;
        while(curr) { allEvents.push_back(curr->data); curr = curr->next; }
        collectAll(node->left);
        collectAll(node->right);
    };
    collectAll(calendarSystem.getBST()->root);

    if(allEvents.empty()) { eventList->addItem(new QListWidgetItem("No events found")); return; }

    // SORTING FIX: Sort ALL events
    std::sort(allEvents.begin(), allEvents.end(), [](const Event& a, const Event& b) {
        QDate dateA = QDate::fromString(QString::fromStdString(a.date), "d:M:yyyy");
        if(!dateA.isValid()) dateA = QDate::fromString(QString::fromStdString(a.date), "dd:MM:yyyy");

        QDate dateB = QDate::fromString(QString::fromStdString(b.date), "d:M:yyyy");
        if(!dateB.isValid()) dateB = QDate::fromString(QString::fromStdString(b.date), "dd:MM:yyyy");

        if (dateA != dateB) return dateA < dateB;
        return a.startTime < b.startTime;
    });

    for (const auto& e : allEvents) {
        QListWidgetItem *item = new QListWidgetItem(); QWidget *container = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(container); layout->setContentsMargins(0, 5, 0, 5);
        QLabel *dot = new QLabel("●"); dot->setStyleSheet("color: #3498db; font-size: 18px;");
        QVBoxLayout *infoLayout = new QVBoxLayout();
        QLabel *title = new QLabel(QString::fromStdString(e.title)); title->setStyleSheet("font-weight: bold; font-size: 14px; color: white;");
        QLabel *time = new QLabel(QString::fromStdString(e.date + " | " + e.startTime)); time->setStyleSheet("color: #ccc; font-size: 12px;");
        infoLayout->addWidget(title); infoLayout->addWidget(time); layout->addWidget(dot); layout->addLayout(infoLayout); layout->addStretch();
        item->setSizeHint(container->sizeHint()); item->setData(Qt::UserRole, QString::fromStdString(e.id));
        eventList->addItem(item); eventList->setItemWidget(item, container);
    }
}

void MainWindow::onSearchTextChanged(const QString &text) {
    if (text.isEmpty()) { onDateSelected(); return; }
    dateDetailLabel->setText("SEARCH RESULTS"); eventList->clear();
    std::vector<Event> results; searchRecursive(calendarSystem.getBST()->root, text.toStdString(), results);
    if(results.empty()) { eventList->addItem(new QListWidgetItem("No matches found")); return; }
    for (const auto& e : results) {
        QListWidgetItem *item = new QListWidgetItem(); QWidget *container = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(container); layout->setContentsMargins(0, 5, 0, 5);
        QLabel *dot = new QLabel("●"); dot->setStyleSheet("color: white; font-size: 18px;");
        QVBoxLayout *infoLayout = new QVBoxLayout();
        QLabel *title = new QLabel(QString::fromStdString(e.title)); title->setStyleSheet("font-weight: bold; font-size: 14px; color: white;");
        QLabel *time = new QLabel(QString::fromStdString(e.date + " | " + e.startTime)); time->setStyleSheet("color: #ccc; font-size: 12px;");
        infoLayout->addWidget(title); infoLayout->addWidget(time); layout->addWidget(dot); layout->addLayout(infoLayout); layout->addStretch();
        item->setSizeHint(container->sizeHint()); item->setData(Qt::UserRole, QString::fromStdString(e.id));
        eventList->addItem(item); eventList->setItemWidget(item, container);
    }
}

void MainWindow::onAddEventClicked() {
    AddEventDialog dialog(this); dialog.setDate(calendar->selectedDate());
    if (dialog.exec() == QDialog::Accepted) {
        if(calendarSystem.addEvent(dialog.getTitle(), dialog.getDate(), dialog.getTime(), dialog.getDuration(), dialog.getPriority(), dialog.getDescription())) {
            saveData();
            if (dateDetailLabel->text() == "ALL EVENTS") onViewAllClicked();
            else if (dateDetailLabel->text().contains("NEXT")) onUpcomingClicked();
            else onDateSelected();
        } else {
            QMessageBox::warning(this, "Conflict", "Time Conflict Detected! Check console for details.");
        }
    }
}

void MainWindow::onDeleteEventClicked() {
    QListWidgetItem *item = eventList->currentItem();
    if(!item) return;

    QString id = item->data(Qt::UserRole).toString();
    if(id.isEmpty()) return;

    if(QMessageBox::question(this, "Confirm", "Delete this event?") == QMessageBox::Yes) {
        bool success = calendarSystem.deleteEvent(id.toStdString(), true);

        if (success) {
            saveData();
            if (!searchBar->text().isEmpty()) onSearchTextChanged(searchBar->text());
            else if (dateDetailLabel->text() == "ALL EVENTS") onViewAllClicked();
            else if (dateDetailLabel->text().contains("NEXT")) onUpcomingClicked();
            else onDateSelected();
        } else {
            QMessageBox::warning(this, "Error", "Could not delete event. ID not found in backend.");
        }
    }
}

void MainWindow::onEditEventClicked() {
    QListWidgetItem *item = eventList->currentItem(); if(!item) return;
    QString id = item->data(Qt::UserRole).toString(); if(id.isEmpty()) return;
    Event* evt = calendarSystem.getEventById(id.toStdString()); if(!evt) return;
    AddEventDialog dialog(this); dialog.setWindowTitle("Edit Event");

    dialog.setEventData(QString::fromStdString(evt->title), QDate::fromString(QString::fromStdString(evt->date), "dd:MM:yyyy"), QTime::fromString(QString::fromStdString(evt->startTime), "HH:mm"), evt->durationMins, evt->priority);

    if (dialog.exec() == QDialog::Accepted) {
        calendarSystem.deleteEvent(id.toStdString(), true);

        if(calendarSystem.addEvent(dialog.getTitle(), dialog.getDate(), dialog.getTime(), dialog.getDuration(), dialog.getPriority(), dialog.getDescription())) {
            saveData();
            if (dateDetailLabel->text() == "ALL EVENTS") onViewAllClicked();
            else if (dateDetailLabel->text().contains("NEXT")) onUpcomingClicked();
            else onDateSelected();
        } else { QMessageBox::warning(this, "Conflict", "Update failed."); }
    }
}

void MainWindow::onMonthBtnClicked(int monthIndex) {
    int year = calendar->selectedDate().year(); calendar->setCurrentPage(year, monthIndex);
    headerLabel->setText(QLocale().monthName(monthIndex).toUpper()); updateMonthButtons(monthIndex);
}
void MainWindow::updateMonthButtons(int currentMonth) {
    for(auto btn : monthButtons) btn->setChecked(false);
    if(currentMonth >= 1 && currentMonth <= 12) monthButtons[currentMonth - 1]->setChecked(true);
}
void MainWindow::onDateSelected() {
    searchBar->clear(); QDate date = calendar->selectedDate();
    dateDetailLabel->setText(date.toString("MMMM d, yyyy").toUpper());
    headerLabel->setText(QLocale().monthName(date.month()).toUpper());
    updateMonthButtons(date.month());
    refreshEventList(date.toString("dd:MM:yyyy").toStdString());
}
void MainWindow::searchRecursive(BSTNode* node, const std::string& query, std::vector<Event>& results) {
    if (!node) return; searchRecursive(node->left, query, results);
    ListNode* curr = node->events.head;
    while(curr) {
        QString title = QString::fromStdString(curr->data.title).toLower();
        QString id = QString::fromStdString(curr->data.id).toLower();
        QString q = QString::fromStdString(query).toLower();
        if (title.contains(q) || id.contains(q)) results.push_back(curr->data);
        curr = curr->next;
    }
    searchRecursive(node->right, query, results);
}
void MainWindow::refreshEventList(const std::string& dateStr) {
    eventList->clear(); BSTNode* node = calendarSystem.getBST()->search(dateStr);
    if(!node) { QListWidgetItem *item = new QListWidgetItem("No events"); item->setTextAlignment(Qt::AlignCenter); eventList->addItem(item); return; }
    ListNode* curr = node->events.head;
    while(curr) {
        QListWidgetItem *item = new QListWidgetItem(); QWidget *container = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(container); layout->setContentsMargins(0, 5, 0, 5);
        QLabel *dot = new QLabel("●"); QString color = (curr->data.priority == 1) ? "#ff6b6b" : "#feca57";
        dot->setStyleSheet("color: " + color + "; font-size: 18px;");
        QVBoxLayout *infoLayout = new QVBoxLayout();
        QLabel *title = new QLabel(QString::fromStdString(curr->data.title)); title->setStyleSheet("font-weight: bold; font-size: 14px; color: white;");
        QLabel *time = new QLabel(QString::fromStdString(curr->data.startTime + " - " + curr->data.getEndTime())); time->setStyleSheet("color: #ccc; font-size: 12px;");
        infoLayout->addWidget(title); infoLayout->addWidget(time); layout->addWidget(dot); layout->addLayout(infoLayout); layout->addStretch();
        item->setSizeHint(container->sizeHint()); item->setData(Qt::UserRole, QString::fromStdString(curr->data.id));
        eventList->addItem(item); eventList->setItemWidget(item, container); curr = curr->next;
    }
}
