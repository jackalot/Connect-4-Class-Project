// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <QDebug>
#include <QMessageBox>

#include "ui_mainwindow.h"
#include "Connect4.h"
#include "battleship.h"
#include "tictactoe.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_OnePlayerButtonBS_clicked();
    void on_OnePlayerButtonC4_clicked();
    void on_TwoPlayerButtonC4_clicked();
    void on_OnePlayerButtonTTT_clicked();
    void on_TwoPlayerButtonTTT_clicked();

private:
    // Enhanced initialization method
    void setupConnections();

    Ui::MainWindow *ui;
    // make an instance of this, we will reveal this window in mainwindow.cpp
    // Use std::unique_ptr for better memory management
    std::unique_ptr<Connect4> newConnect4; // Change to std::unique_ptr
    std::unique_ptr<battleship> newBattleship; // Change to std::unique_ptr
    std::unique_ptr<TicTacToe> newTicTacToe; // Change to std::unique_ptr
};

#endif // MAINWINDOW_H
