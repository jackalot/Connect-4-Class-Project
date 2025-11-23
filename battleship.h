#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <QDialog>

namespace Ui {
class battleship;
}

class battleship : public QDialog
{
    Q_OBJECT

public:
    explicit battleship(QWidget *parent = nullptr);
    ~battleship();

    void HighlightCell(int row, int col, char ColorKey); // Make this public

private slots:
    void onButtonClicked(); // Keep this under private slots

    void on_UndoButton_clicked();

private:
    Ui::battleship *ui;
};

#endif // BATTLESHIP_H
