#ifndef Dialog_AddCity_H
#define Dialog_AddCity_H

#include <QDialog>
#include "City.h"

namespace Ui {
class Dialog_AddCity;
}

class Dialog_AddCity : public QDialog {
    Q_OBJECT

public:
    explicit Dialog_AddCity(QWidget *parent = nullptr);
    ~Dialog_AddCity();

signals:
    void newCityDiscovered(City newCity);

private slots:
    void on_lineEdit_cityName_textChanged(const QString &arg1);
    void on_comboBox_cityColor_currentIndexChanged(const QString &arg1);
    void on_pushButton_addCity_clicked();
    void on_pushButton_cancel_clicked();
    void on_comboBox_cityLevel_currentIndexChanged(int index);

private:
    Ui::Dialog_AddCity *ui;
    City mCity;

    const std::vector<int> greenCityMonsterList = {0, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5};
    const std::vector<int> blueCityMonsterList  = {0, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5};
    const std::vector<int> redCityMonsterList   = {0, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5};
    const std::vector<int> whiteCityMonsterList = {0, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5};

    void ui_dialogSetupClean();
    void dialogAcceptEnableCheck();
    void updateCityMonsterCount();
};

#endif // Dialog_AddCity_H
