#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Game)
{
    ui->setupUi(this);
}

Game::~Game()
{
    delete ui;
}
/*=====================
 * Row 1
 ====================*/
void Game::on_Coll1R1_clicked()
{
    //Collumn 1 Row 1
}


void Game::on_Coll2R1_clicked()
{

}

