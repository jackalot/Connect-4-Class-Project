#include "mainwindow.h"
#include "./ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
/*
Battle Ship
*/
void MainWindow::on_OnePlayerButtonBS_clicked()
{

}

/*
Connect 4
*/
void MainWindow::on_OnePlayerButtonC4_clicked()
{
    // Hides main window and then reveals gameWindow
    hide();
    newGame = new Game(this);
    newGame->show();
}


void MainWindow::on_TwoPlayerButtonC4_clicked()
{
    // Hides main window and then reveals gameWindow
    hide();
    newGame = new Game(this);
    newGame->show();
}

/*
 * Tic Tack Toe
 */
void MainWindow::on_OnePlayerButtonTTT_clicked()
{

}


void MainWindow::on_TwoPlayerButtonTTT_clicked()
{

}

