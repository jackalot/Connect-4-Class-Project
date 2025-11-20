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
void battleship::HighlightCell(int row, int col)
{
    /*Create the object of the clicked button building
    the row and collumn into "Coll%1R%2"*/
    QString buttonName = QString("Coll%1R%2").arg(col).arg(row);

    //Find the button for the newly created object
    QPushButton* button = this->findChild<QPushButton*>(buttonName);
    button->setStyleSheet("background-color: green; color: white;");
}
battleship::~battleship()
{
    delete ui;
}

void battleship::onButtonClicked() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString buttonName = button->objectName();
        //Use regex to parse out row and collumn
        static QRegularExpression regex("Coll(\\d+)R(\\d+)");
        QRegularExpressionMatch match = regex.match(buttonName);

        // Your code based on buttonName
        int col = match.captured(1).toInt();
        int row = match.captured(2).toInt();
        HighlightCell(row, col);
    }
}
