#ifndef Window_StartMenu_H
#define Window_StartMenu_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <vector>
#include "Window_MainBoard.h"

namespace Ui {
class Window_StartMenu;
}

class Window_StartMenu : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor
    explicit Window_StartMenu(QWidget *parent = nullptr);
    // Destructor
    ~Window_StartMenu();

private slots:
    void on_pushButton_newGame_clicked();
    void on_pushButton_returnToStart_clicked();
    void on_pushButton_Exit_clicked();
    void on_pushButton_startGame_clicked();
    void on_pushButton_addPlayer_clicked();
    void on_comboBox_playerCharacter_currentIndexChanged(const QString &arg1);
    void on_lineEdit_playerName_textChanged(const QString &arg1);
    void on_pushButton_loadGame_clicked();
    void on_spinBox_dayCount_valueChanged(int arg1);

private:

    // Private members
    Ui::Window_StartMenu *ui;
    Window_MainBoard *mWindow_MainBoard;
    QStringList m_TableHeader;
    QString mStyle_CharacterClean;
    QString mStyle_CharacterArythea;
    QString mStyle_CharacterGoldyx;
    QString mStyle_CharacterNorowas;
    QString mStyle_CharacterTovak;
    QString mStyle_CharacterKrang;

    void ui_newGameSetupClean();
    void startButtonControlCheck();
};

#endif // Window_StartMenu_H
