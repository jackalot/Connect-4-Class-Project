#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory> // For smart pointers
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
    std::unique_ptr<Game> newGame; // Use smart pointer
    std::unique_ptr<battleship> newBattleship; // Use smart pointer
};

#endif // MAINWINDOW_H
