#include "battleship.h"
#include "ui_battleship.h"

battleship::battleship(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::battleship)
{
    ui->setupUi(this);

    foreach (const QString &buttonName, buttonNames) {
        QRegularExpression regex("Coll(\\d+)R(\\d+)");
        QList<QPushButton*> gridButtons = this->findChildren<QPushButton*>(regex);
        if (button) {
            connect(gridButtons, &QPushButton::clicked, this, &battleship::onButtonClicked);
        }
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
