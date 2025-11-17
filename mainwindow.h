#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include "game.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
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
    // make an instance of this, we will reveal this window in mainwindow.cpp
    Game *newGame;
};
#endif // MAINWINDOW_H
