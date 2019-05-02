#ifndef DIALOG_STARTMENU_H
#define DIALOG_STARTMENU_H

#include <QDialog>
#include "Player.h"
#include "City.h"

namespace Ui {
class Dialog_StartMenu;
}

class Dialog_StartMenu : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit Dialog_StartMenu(QWidget *parent = nullptr);
    // Destructor
    ~Dialog_StartMenu();

signals:
    void playerAdded(QString playerName, QString playerCharacter);
    void resetGame();    // If the user returns from the "New Game" menu back to the start page
    void startNewGame(bool Volkare_isPresent, City Volkare); // When the user presses "Start" to trigger the actual game start

public slots:
    void on_playerListUpdated(const std::vector<Player>& mageKnight_Players);

private slots:
    void on_pushButton_newGame_clicked();
    void on_pushButton_loadGame_clicked();
    void on_pushButton_Exit_clicked();
    void on_pushButton_returnToStart_clicked();
    void on_pushButton_startGame_clicked();
    void on_pushButton_addPlayer_clicked();
    void on_checkBox_Volkare_clicked();
    void on_spinBox_VolkareLevel_valueChanged(int arg1);
    void on_spinBox_VolkareLegion_valueChanged(int arg1);

    void on_comboBox_playerCharacter_currentIndexChanged(const QString &arg1);
    void on_lineEdit_playerName_textChanged(const QString &arg1);

private:
    Ui::Dialog_StartMenu *ui;
    QStringList m_TableHeader;
    City Volkare;

    void ui_newGameSetupClean();
    void startButtonControlCheck();

};

#endif // DIALOG_STARTMENU_H
