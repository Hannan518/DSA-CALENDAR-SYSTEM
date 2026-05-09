#ifndef EVENTCALENDAR_H
#define EVENTCALENDAR_H

#include <QCalendarWidget>
#include <QSet>
#include <QDate>

class EventCalendar : public QCalendarWidget {
    Q_OBJECT

public:
    using QCalendarWidget::QCalendarWidget;

    // This function is kept empty so we don't break mainwindow calls,
    // but it no longer draws anything.
    void setEventDates(const QSet<QDate>& dates) {
        // Feature removed: No dots will be drawn.
        update();
    }
};

#endif
