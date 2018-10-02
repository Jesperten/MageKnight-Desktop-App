#ifndef Dialog_EndGame_H
#define Dialog_EndGame_H

#include <QDialog>

namespace Ui {
class Dialog_EndGame;
}

class Dialog_EndGame : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_EndGame(QWidget *parent = nullptr);
    ~Dialog_EndGame();

private slots:
    void on_pushButton_no_clicked();

    void on_pushButton_yes_clicked();

private:
    Ui::Dialog_EndGame *ui;
};

#endif // Dialog_EndGame_H
