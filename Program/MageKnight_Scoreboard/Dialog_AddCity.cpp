#include "Dialog_AddCity.h"
#include "ui_Dialog_AddCity.h"

Dialog_AddCity::Dialog_AddCity(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog_AddCity)
{
    ui->setupUi(this);
    ui_dialogSetupClean();

    mCity.mDiscovered = true;
}

Dialog_AddCity::~Dialog_AddCity()
{
    delete ui;
}


void Dialog_AddCity::ui_dialogSetupClean()
{
    ui->lineEdit_cityName->setPlaceholderText("City Name");
    ui->pushButton_addCity->setEnabled(false);
}

void Dialog_AddCity::on_lineEdit_cityName_textChanged(const QString &arg1)
{
    mCity.mName = arg1;
    dialogAcceptEnableCheck();
}

void Dialog_AddCity::dialogAcceptEnableCheck()
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

void Dialog_AddCity::on_comboBox_cityColor_currentIndexChanged(const QString &arg1)
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

void Dialog_AddCity::on_comboBox_cityLevel_currentIndexChanged(int index)
{
    mCity.mLevel = index;
    updateCityMonsterCount();
    dialogAcceptEnableCheck();
}

void Dialog_AddCity::on_pushButton_addCity_clicked()
{
    this->accept();
}

void Dialog_AddCity::on_pushButton_cancel_clicked()
{
    this->reject();
}


void Dialog_AddCity::updateCityMonsterCount()
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
