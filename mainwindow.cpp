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
//Press Start buttons
void MainWindow::on_OnePlayerButton_clicked()
{
    // Hides main window and then reveals gameWindow
    hide();
    newGame = new Game(this);
    newGame->show();
}


void MainWindow::on_TwoPlayerButton_clicked()
{
    /* Press start for p1 vs p2*/
    // Hides main window and then reveals gameWindow
    hide();
    newGame = new Game(this);
    newGame->show();
}

