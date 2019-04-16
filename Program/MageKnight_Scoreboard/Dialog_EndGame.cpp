#include "Dialog_EndGame.h"
#include "ui_Dialog_EndGame.h"

Dialog_EndGame::Dialog_EndGame(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog_EndGame)
{
    ui->setupUi(this);
}

Dialog_EndGame::~Dialog_EndGame()
{
    delete ui;
}

void Dialog_EndGame::on_pushButton_no_clicked()
{
    this->reject();
}

void Dialog_EndGame::on_pushButton_yes_clicked()
{
    this->accept();
}
