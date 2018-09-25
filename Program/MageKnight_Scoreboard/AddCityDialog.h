#ifndef ADDCITYDIALOG_H
#define ADDCITYDIALOG_H

#include <QDialog>
#include "City.h"

namespace Ui {
class AddCityDialog;
}

class AddCityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddCityDialog(QWidget *parent = 0);
    ~AddCityDialog();
    City mCity;

private slots:
    void on_lineEdit_cityName_textChanged(const QString &arg1);
    void on_comboBox_cityColor_currentIndexChanged(const QString &arg1);
    void on_pushButton_addCity_clicked();
    void on_pushButton_cancel_clicked();
    void on_comboBox_cityLevel_currentIndexChanged(int index);

private:
    Ui::AddCityDialog *ui;
    std::vector<int> greenCityMonsterList = {0, 2, 2, 3, 3 ,3 ,4, 4, 4, 5, 5, 5};
    std::vector<int> blueCityMonsterList  = {0, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5};
    std::vector<int> redCityMonsterList   = {0, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5};
    std::vector<int> whiteCityMonsterList = {0, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5};

    void ui_dialogSetupClean();
    void dialogAcceptEnableCheck();
    void updateCityMonsterCount();
};

#endif // ADDCITYDIALOG_H
