#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include "Connect4.h"
#include "battleship.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_OnePlayerButtonBS_clicked();
    void on_OnePlayerButtonC4_clicked();
    void on_TwoPlayerButtonC4_clicked();
    void on_OnePlayerButtonTTT_clicked();
    void on_TwoPlayerButtonTTT_clicked();

private:
    Ui::MainWindow *ui;
    // Use pointers to the different game instances
    Game *newGame = nullptr; // Initialize to nullptr
    battleship *newBattleship = nullptr; // Initialize to nullptr
};

#endif // MAINWINDOW_H
