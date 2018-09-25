#ifndef AREYOURSUREDIALOG_H
#define AREYOURSUREDIALOG_H

#include <QDialog>

namespace Ui {
class AreYourSureDialog;
}

class AreYourSureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AreYourSureDialog(QWidget *parent = 0);
    ~AreYourSureDialog();

private slots:
    void on_pushButton_no_clicked();

    void on_pushButton_yes_clicked();

private:
    Ui::AreYourSureDialog *ui;
};

#endif // AREYOURSUREDIALOG_H
