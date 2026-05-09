#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H
#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QSpinBox>
#include <QComboBox>
class AddEventDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddEventDialog(QWidget *parent = nullptr);
    void setDate(QDate date);
    void setEventData(QString title, QDate date, QTime time, int dur, int prio);
    std::string getTitle() const;
    std::string getDate() const;
    std::string getTime() const;
    int getDuration() const;
    int getPriority() const;
    std::string getDescription() const;
private:
    QLineEdit *titleEdit; QDateEdit *dateEdit; QTimeEdit *timeEdit;
    QSpinBox *durationSpin; QComboBox *priorityCombo; QLineEdit *descEdit;
};
#endif
