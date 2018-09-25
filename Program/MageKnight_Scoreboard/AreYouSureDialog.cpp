#include "AreYouSureDialog.h"
#include "ui_AreYouSureDialog.h"

AreYourSureDialog::AreYourSureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AreYourSureDialog)
{
    ui->setupUi(this);
}

AreYourSureDialog::~AreYourSureDialog()
{
    delete ui;
}

void AreYourSureDialog::on_pushButton_no_clicked()
{
    this->reject();
}

void AreYourSureDialog::on_pushButton_yes_clicked()
{
    this->accept();
}
