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
    std::unique_ptr<Game> newGame;
    std::unique_ptr<battleship> newBattleship;
};

#endif // MAINWINDOW_H
