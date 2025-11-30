// mainwindow.cpp
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , newConnect4(nullptr)
    , newBattleship(nullptr)
    , newTicTacToe(nullptr)
{
    try {
        qDebug() << "MainWindow: Starting initialization";

        // Validate UI pointer
        if (!ui) {
            throw std::runtime_error("Failed to create UI");
        }

        // Setup UI
        ui->setupUi(this);

        // Explicit connection setup
        setupConnections();

        qDebug() << "MainWindow: Initialization completed";
    }
    catch (const std::exception& e) {
        qDebug() << "MainWindow Initialization Error: " << e.what();
        QMessageBox::critical(this,
                              "Initialization Error",
                              QString("Failed to initialize main window: %1").arg(e.what())
                              );
    }
}

void MainWindow::setupConnections()
{
    try {
        // Explicit Qt5/Qt6 compatible signal-slot connections
        connect(ui->OnePlayerButtonBS, &QPushButton::clicked,
                this, &MainWindow::on_OnePlayerButtonBS_clicked);

        connect(ui->OnePlayerButtonC4, &QPushButton::clicked,
                this, &MainWindow::on_OnePlayerButtonC4_clicked);

        connect(ui->TwoPlayerButtonC4, &QPushButton::clicked,
                this, &MainWindow::on_TwoPlayerButtonC4_clicked);

        connect(ui->OnePlayerButtonTTT, &QPushButton::clicked,
                this, &MainWindow::on_OnePlayerButtonTTT_clicked);

        connect(ui->TwoPlayerButtonTTT, &QPushButton::clicked,
                this, &MainWindow::on_TwoPlayerButtonTTT_clicked);

        qDebug() << "MainWindow: All button connections established";
    }
    catch (const std::exception& e) {
        qDebug() << "Button Connection Error: " << e.what();
        QMessageBox::critical(this,
                              "Connection Error",
                              QString("Failed to connect buttons: %1").arg(e.what())
                              );
    }
}

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow: Destructor called";
    delete ui;
}

// Existing button handlers remain the same


void MainWindow::on_OnePlayerButtonBS_clicked()
{
    try {
        qDebug() << "OnePlayerButtonBS clicked";

        // Validate current window state
        if (!isVisible()) {
            qDebug() << "Cannot open Battleship from invisible window";
            return;
        }

        // Hide current window
        hide();

        // Create Battleship window
        try {
            newBattleship = std::make_unique<battleship>(this);

            // Validate object creation
            if (!newBattleship) {
                throw std::runtime_error("Failed to create Battleship window");
            }

            newBattleship->show();
        }
        catch (const std::exception& e) {
            qDebug() << "Battleship Window Creation Error: " << e.what();

            // Restore main window visibility
            show();

            // Show error to user
            QMessageBox::critical(this,
                                  "Window Creation Error",
                                  QString("Could not open Battleship game: %1").arg(e.what())
                                  );
        }
    }
    catch (...) {
        qDebug() << "Unexpected error in Battleship button handler";
        show(); // Ensure main window is visible
    }
}

void MainWindow::on_OnePlayerButtonC4_clicked()
{
    try {
        qDebug() << "OnePlayerButtonC4 clicked";

        // Validate current window state
        if (!isVisible()) {
            qDebug() << "Cannot open Connect 4 from invisible window";
            return;
        }

        // Hide current window
        hide();


        // Create Connect 4 window
        try {
            newConnect4 = std::make_unique<Connect4>(this);

            // Validate object creation
            if (!newConnect4) {
                throw std::runtime_error("Failed to create Connect 4 window");
            }

            newConnect4->show();
        }
        catch (const std::exception& e) {
            qDebug() << "Connect 4 Window Creation Error: " << e.what();

            // Restore main window visibility
            show();

            // Show error to user
            QMessageBox::critical(this,
                                  "Window Creation Error",
                                  QString("Could not open Connect 4 game: %1").arg(e.what())
                                  );
        }
    }
    catch (...) {
        qDebug() << "Unexpected error in Connect 4 button handler";
        show(); // Ensure main window is visible
    }
    // Hides main window and then reveals gameWindow
    hide();
    newConnect4 = std::make_unique<Connect4>(this, true);
    newConnect4->show();
}
void MainWindow::on_TwoPlayerButtonC4_clicked()
{
    try {
        qDebug() << "TwoPlayerButtonC4 clicked";

        // Hide main window
        hide();

        // Create Connect 4 window in TWO-PLAYER mode
        newConnect4 = std::make_unique<Connect4>(this, false); // <-- false = two-player
        if (!newConnect4) {
            throw std::runtime_error("Failed to create Connect 4 window");
        }

        newConnect4->show();
    }
    catch (const std::exception& e) {
        qDebug() << "Connect 4 Window Creation Error: " << e.what();
        show(); // restore main window
        QMessageBox::critical(this,
                              "Window Creation Error",
                              QString("Could not open Connect 4 game: %1").arg(e.what())
                              );
    }
}


void MainWindow::on_OnePlayerButtonTTT_clicked()
{
    try {
        qDebug() << "OnePlayerButtonBS clicked";

        // Validate current window state
        if (!isVisible()) {
            qDebug() << "Cannot open Tic Tac Toe from invisible window";
            return;
        }

        // Hide current window
        hide();

        // Create Battleship window
        try {
            newTicTacToe = std::make_unique<TicTacToe>(this, true);

            // Validate object creation
            if (!newTicTacToe) {
                throw std::runtime_error("Failed to create Tic Tac Toe window");
            }
            newTicTacToe->show();
        }
        catch (const std::exception& e) {
            qDebug() << "Tic Tac Toe Window Creation Error: " << e.what();

            // Restore main window visibility
            show();

            // Show error to user
            QMessageBox::critical(this,
                                  "Window Creation Error",
                                  QString("Could not open Tic Tac Toe game: %1").arg(e.what())
                                  );
        }
    }
    catch (...) {
        qDebug() << "Unexpected error in Tic Tac Toe button handler";
        show(); // Ensure main window is visible
    }
}

void MainWindow::on_TwoPlayerButtonTTT_clicked()
{
    try {
        qDebug() << "OnePlayerButtonBS clicked";

        // Validate current window state
        if (!isVisible()) {
            qDebug() << "Cannot open Tic Tac Toe from invisible window";
            return;
        }

        // Hide current window
        hide();

        // Create Battleship window
        try {
            newTicTacToe = std::make_unique<TicTacToe>(this, false);

            // Validate object creation
            if (!newTicTacToe) {
                throw std::runtime_error("Failed to create Tic Tac Toe window");
            }
            newTicTacToe->show();
        }
        catch (const std::exception& e) {
            qDebug() << "Tic Tac Toe Window Creation Error: " << e.what();

            // Restore main window visibility
            show();

            // Show error to user
            QMessageBox::critical(this,
                                  "Window Creation Error",
                                  QString("Could not open Tic Tac Toe game: %1").arg(e.what())
                                  );
        }
    }
    catch (...) {
        qDebug() << "Unexpected error in Tic Tac Toe button handler";
        show(); // Ensure main window is visible
    }
}
