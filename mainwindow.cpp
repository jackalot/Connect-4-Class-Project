#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , newConnect4(nullptr)
    , newBattleship(nullptr)
    , newTicTacToe(nullptr)
{
    ui->setupUi(this);
}
MainWindow::~MainWindow()
{
    delete ui;
    if (newTicTacToe) delete newTicTacToe;
    if (newBattleship) delete newBattleship;
    if (newConnect4) delete newConnect4;
}

/*

Battle Ship

*/

void MainWindow::on_OnePlayerButtonBS_clicked()
{
    // Hides main window and then reveals gameWindow
    hide();
    if (newBattleship) {
        delete newBattleship;
        newBattleship = nullptr;
    }
    newBattleship = new battleship(this);
    //Allows main window to reopen after Battlehsip is closed
    connect(newBattleship, &QDialog::finished, this, &MainWindow::show);
    newBattleship->show();
}

/*

Connect 4

*/

void MainWindow::on_OnePlayerButtonC4_clicked()
{
    // Hides main window and then reveals gameWindow
    hide();
    if (newConnect4) {
        delete newConnect4;
        newConnect4 = nullptr;
    }
    newConnect4 = new Connect4(this, true);
    //Allows main window to reopen after Connect 4 is closed
    connect(newConnect4, &QDialog::finished, this, &MainWindow::show);
    newConnect4->show();
}
void MainWindow::on_TwoPlayerButtonC4_clicked()
{
    // Hides main window and then reveals gameWindow
    hide();
    if (newConnect4) {
        delete newConnect4;
        newConnect4 = nullptr;
    }
    newConnect4 = new Connect4(this, false);
    connect(newConnect4, &QDialog::finished, this, &MainWindow::show);
    newConnect4->show();
}
/*

 * Tic Tack Toe

 */
void MainWindow::on_OnePlayerButtonTTT_clicked()
{
    hide();
    if (newTicTacToe) {
        delete newTicTacToe;
        newTicTacToe = nullptr;
    }
    newTicTacToe = new TicTacToe(this, true);    //Allows main window to reopen after tictactoe is closed
    connect(newTicTacToe, &QDialog::finished, this, &MainWindow::show);
    newTicTacToe->show();
}

void MainWindow::on_TwoPlayerButtonTTT_clicked()
{
    hide();
    if (newTicTacToe) {
        delete newTicTacToe;
        newTicTacToe = nullptr;
    }
    newTicTacToe = new TicTacToe(this, false);
    connect(newTicTacToe, &QDialog::finished, this, &MainWindow::show);




    newTicTacToe->show();

}
