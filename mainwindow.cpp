#include "mainwindow.h"
#include "./ui_mainwindow.h"

// Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    newGame(nullptr),  // Initialize to nullptr
    newBattleship(nullptr) // Initialize to nullptr
{
    ui->setupUi(this);
}

// Destructor
MainWindow::~MainWindow()
{
    delete ui; // Clean up the UI
    delete newGame; // Clean up Game pointer
    delete newBattleship; // Clean up battleship pointer
}

/*
 * Battle Ship
 */
void MainWindow::on_OnePlayerButtonBS_clicked()
{
    hide(); // Hide main window
    if (newBattleship) {
        delete newBattleship; // Clean up previous instance if it exists
    }
    newBattleship = new battleship(this); // Allocate new battleship
    newBattleship->show(); // Show battleship window
}

/*
 * Connect 4
 */
void MainWindow::on_OnePlayerButtonC4_clicked()
{
    hide(); // Hide main window
    if (newGame) {
        delete newGame; // Clean up previous instance if it exists
    }
    newGame = new Game(this); // Allocate new Game instance
    newGame->show(); // Show game window
}

void MainWindow::on_TwoPlayerButtonC4_clicked()
{
    hide(); // Hide main window
    if (newGame) {
        delete newGame; // Clean up previous instance if it exists
    }
    newGame = new Game(this); // Allocate new Game instance
    newGame->show(); // Show game window
}

/*
 * Tic Tac Toe
 */
void MainWindow::on_OnePlayerButtonTTT_clicked()
{
    // Implementation for single-player Tic Tac Toe
}

void MainWindow::on_TwoPlayerButtonTTT_clicked()
{
    // Implementation for two-player Tic Tac Toe
}
