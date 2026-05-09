#include "addeventdialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QPushButton>
AddEventDialog::AddEventDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Add New Event");
    setStyleSheet("background-color: white; color: #2d3436;");
    QFormLayout *layout = new QFormLayout(this);
    titleEdit = new QLineEdit();
    dateEdit = new QDateEdit(QDate::currentDate()); dateEdit->setDisplayFormat("dd:MM:yyyy"); // Updated format
    timeEdit = new QTimeEdit(QTime::currentTime()); timeEdit->setDisplayFormat("HH:mm");
    durationSpin = new QSpinBox(); durationSpin->setRange(1, 1440); durationSpin->setValue(60);
    priorityCombo = new QComboBox(); priorityCombo->addItems({"1 - High", "2 - Medium", "3 - Low", "4 - Lower", "5 - Lowest"});
    descEdit = new QLineEdit();
    layout->addRow("Title:", titleEdit); layout->addRow("Date:", dateEdit); layout->addRow("Start Time:", timeEdit);
    layout->addRow("Duration (mins):", durationSpin); layout->addRow("Priority:", priorityCombo); layout->addRow("Description:", descEdit);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
void AddEventDialog::setDate(QDate date) { dateEdit->setDate(date); }
void AddEventDialog::setEventData(QString title, QDate date, QTime time, int dur, int prio) {
    titleEdit->setText(title); dateEdit->setDate(date); timeEdit->setTime(time); durationSpin->setValue(dur);
    priorityCombo->setCurrentIndex(prio - 1);
}
std::string AddEventDialog::getTitle() const { return titleEdit->text().toStdString(); }
std::string AddEventDialog::getDate() const { return dateEdit->date().toString("dd:MM:yyyy").toStdString(); } // Updated format
std::string AddEventDialog::getTime() const { return timeEdit->time().toString("HH:mm").toStdString(); }
int AddEventDialog::getDuration() const { return durationSpin->value(); }
int AddEventDialog::getPriority() const { return priorityCombo->currentIndex() + 1; }
std::string AddEventDialog::getDescription() const { return descEdit->text().toStdString(); }
