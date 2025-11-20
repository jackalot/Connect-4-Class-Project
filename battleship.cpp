#include "battleship.h"
#include "ui_battleship.h"

battleship::battleship(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::battleship)
{
    ui->setupUi(this);

    QStringList buttonNames = {
        "Coll1R1", "Coll1R2", "Coll2R1", "Coll2R2",
        "Coll1R3", "Coll1R4", "Coll2R3", "Coll2R4"
        // Add all button names here
    };

    foreach (const QString &buttonName, buttonNames) {
        QPushButton *button = this->findChild<QPushButton *>(buttonName);
        if (button) {
            connect(button, &QPushButton::clicked, this, &battleship::onButtonClicked);
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
