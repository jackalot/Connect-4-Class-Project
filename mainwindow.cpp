#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "game.h"
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
    Game mainGame;
    mainGame.setModal();
}


void MainWindow::on_TwoPlayerButton_clicked()
{
    /* Press start for p1 vs p2*/
}

