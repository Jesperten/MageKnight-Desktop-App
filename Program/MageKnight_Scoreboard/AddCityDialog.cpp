#include "AddCityDialog.h"
#include "ui_AddCityDialog.h"

AddCityDialog::AddCityDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCityDialog)
{
    ui->setupUi(this);
    ui_dialogSetupClean();

    mCity.mDiscovered = true;
}

AddCityDialog::~AddCityDialog()
{
    delete ui;
}


void AddCityDialog::ui_dialogSetupClean()
{
    ui->lineEdit_cityName->setPlaceholderText("City Name");
    ui->pushButton_addCity->setEnabled(false);
}

void AddCityDialog::on_lineEdit_cityName_textChanged(const QString &arg1)
{
    mCity.mName = arg1;
    dialogAcceptEnableCheck();
}

void AddCityDialog::dialogAcceptEnableCheck()
{
    if ((mCity.mName == "") || (mCity.mColor == "") || (mCity.mLevel == 0))
    {
        ui->pushButton_addCity->setEnabled(false);
    }
    else
    {
        ui->pushButton_addCity->setEnabled(true);
    }
}

void AddCityDialog::on_comboBox_cityColor_currentIndexChanged(const QString &arg1)
{
    if (ui->comboBox_cityColor->currentIndex() > 0)
    {
        mCity.mColor = arg1;
    }
    else
    {
        mCity.mColor = "";
    }

    updateCityMonsterCount();
    dialogAcceptEnableCheck();
}

void AddCityDialog::on_comboBox_cityLevel_currentIndexChanged(int index)
{
    mCity.mLevel = index;
    updateCityMonsterCount();
    dialogAcceptEnableCheck();
}

void AddCityDialog::on_pushButton_addCity_clicked()
{
    this->accept();
}

void AddCityDialog::on_pushButton_cancel_clicked()
{
    this->reject();
}


void AddCityDialog::updateCityMonsterCount()
{
    unsigned int index = unsigned(ui->comboBox_cityLevel->currentIndex());

    if (ui->comboBox_cityColor->currentText() == "Green")
    {
        mCity.mMonstersRemaining = greenCityMonsterList.at(index);
    }
    else if (ui->comboBox_cityColor->currentText() == "Blue")
    {
        mCity.mMonstersRemaining = blueCityMonsterList.at(index);
    }
    else if (ui->comboBox_cityColor->currentText() == "White")
    {
        mCity.mMonstersRemaining = whiteCityMonsterList.at(index);
    }
    else if (ui->comboBox_cityColor->currentText() == "Red")
    {
        mCity.mMonstersRemaining = redCityMonsterList.at(index);
    }
    else
    {
        mCity.mMonstersRemaining = 0;
    }

    mCity.mMonsters = mCity.mMonstersRemaining;

    ui->label_MonsterCount->setText(QString::number(mCity.mMonstersRemaining));
}
