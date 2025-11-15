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
    hide();
    newGame = new Game(this);
    newGame->setSinglePlayer(true);   // <-- One Player
    newGame->show();
}

void MainWindow::on_TwoPlayerButton_clicked()
{
    hide();
    newGame = new Game(this);
    newGame->setSinglePlayer(false);  // <-- Two Players
    newGame->show();
}

