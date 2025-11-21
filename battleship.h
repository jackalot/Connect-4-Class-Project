#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <QDialog>

namespace Ui {
class battleship;
}

class battleship : public QDialog
{
    Q_OBJECT
private slots:
    void onButtonClicked();
    void HighlightCell(int row, int col, char ColorKey);
public:
    explicit battleship(QWidget *parent = nullptr);
    ~battleship();

private:
    Ui::battleship *ui;
};

#endif // BATTLESHIP_H
