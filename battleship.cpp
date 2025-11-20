#include "battleship.h"
#include "ui_battleship.h"
#include <QRegularExpression>

battleship::battleship(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::battleship)
{
    ui->setupUi(this);

    // Use a regex to find buttons with names that match the pattern
    QRegularExpression regex("Coll\\d+R\\d+");
    // Find all QPushButtons that match the regex
    QList<QPushButton*> gridButtons = this->findChildren<QPushButton*>(regex);

    // Connect each button's clicked signal to the onButtonClicked slot
    for (QPushButton* button : gridButtons) {
        connect(button, &QPushButton::clicked, this, &battleship::onButtonClicked);
    }
}

battleship::~battleship()
{
    delete ui;
}

void battleship::onButtonClicked() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString buttonName = button->objectName();
        // Your code based on buttonName
    }
}
