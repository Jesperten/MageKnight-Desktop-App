#ifndef Dialog_UserAction_H
#define Dialog_UserAction_H

#include <QDialog>
#include <QString>
#include <QTableWidgetItem>
#include <QSoundEffect>
#include <vector>
#include "Player.h"
#include "Action.h"
#include "Monster.h"
#include "City.h"

typedef enum score_stat_t
{
    SCORE_STAT_ID_STAT,
    SCORE_STAT_ID_SCORE,
    SCORE_STAT_ID_FAME,
    SCORE_STAT_ID_REP,
    SCORE_STAT_ID_LEVEL,
    SCORE_STAT_ID_AAC,
    SCORE_STAT_ID_SPELL,
    SCORE_STAT_ID_ARTIFACT,
    SCORE_STAT_ID_CRYSTALS,
    NUMBER_OF_SCORE_STATS,
}score_stat_t;

namespace Ui {
class Dialog_UserAction;
}

class Dialog_UserAction : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit Dialog_UserAction(QWidget *parent = nullptr);

    // Destructor
    ~Dialog_UserAction();

    // Public members

    // Public methods
    void setDataLists(const std::vector<Player>& playerList, const std::vector<City>& cityList);

signals:
    void newTestUserAction(Action testAction);
    void newUserAction(Action userAction);

private slots:
    void on_dialogSetupDataReceived(const std::vector<Player>& playerList, const std::vector<City>& cityList);
    void on_newPlayerAndCityData(const std::vector<Player>& playerList, const std::vector<City>& cityList);
    void on_newTempPlayerData(const Player& player, const Player& playerTemp);
    void on_pushButton_enterUserAction_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_addMonster_clicked();
    void on_pushButton_deleteMonster_clicked();
    void on_pushButton_addUnit_clicked();
    void on_pushButton_removeUnit_clicked();

    void on_comboBox_playerList_currentIndexChanged(int index);
    void on_comboBox_actionList_currentIndexChanged(int index);
    void on_comboBox_optional_currentIndexChanged(int index);
    void on_comboBox_successFail_currentIndexChanged(const QString &arg1);

    void on_spinBox_woundCounts_valueChanged(int arg1);
    void on_spinBox_addRep_valueChanged(int arg1);
    void on_spinBox_addFame_valueChanged(int arg1);
    void on_spinBox_thrownArtifacts_valueChanged(int arg1);
    void on_spinBox_thrownAAC_valueChanged(int arg1);
    void monsterTableWidget_valueChanged(void);
    void unitTableWidget_valueChanged(void);

    void on_spinBox_addCrystals_valueChanged(int arg1);

private:

    // Private members
    Ui::Dialog_UserAction *ui;
    Action mplayerAction;

    std::vector<QString> mActionList;
    std::vector<QString> mOptionalListDungeon;
    std::vector<QString> mOptionalListLabyrinth;
    std::vector<QString> mOptionalListMaze;
    std::vector<QString> mOptionalListMonsterDen;
    std::vector<QString> mOptionalListRuins;
    std::vector<QString> mOptionalListSpawningGrounds;

    QSoundEffect mSoundEffects[20];

    std::vector<Player> mPlayerListCopy; // List of player objects (resized later)
    std::vector<City> mCityListCopy;     // List of City objects (resized later)

    // Private methods
    void ui_userActionSetupClean(void);
    void acceptButtonControlCheck(void);
    void setOptionalComboBox(int index);
    void updateResults(void);
    void clearScoreTable(void);
    void defineStringLists(void);
    void enableForm(bool enabled);
    void monsterTableAddRow(unsigned int rows);
    void addMonster(void);
    void removeMonster(void);
    void unitTableAddRow(unsigned int rows, Unit unit);
    void addUnit(Unit unit);
    void removeUnit(void);
};

#endif // Dialog_UserAction_H
