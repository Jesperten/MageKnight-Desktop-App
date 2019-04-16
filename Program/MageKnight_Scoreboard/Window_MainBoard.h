#ifndef Window_MainBoard_H
#define Window_MainBoard_H

#include <QMainWindow>
#include "Dialog_UserAction.h"
#include "GameTimer.h"

typedef enum table_items_t
{
    TABLE_ITEM_NAME_ID,
    TABLE_ITEM_CHARACTER,
    TABLE_ITEM_SCORE,
    TABLE_ITEM_LEVEL,
    TABLE_ITEM_FAME,
    TABLE_ITEM_REPUTATION,
    TABLE_ITEM_AAC,
    TABLE_ITEM_SPELLS,
    TABLE_ITEM_ARTIFACTS,
    TABLE_ITEM_CRYSTALS,
    TABLE_ITEM_MONSTERS,
    TABLE_ITEM_WOUNDS,
    TABLE_ITEM_KNOWLEDGE,
    TABLE_ITEM_LOOT,
    TABLE_ITEM_LEADER,
    TABLE_ITEM_CONQUERER,
    TABLE_ITEM_CITY_CONQUERER,
    TABLE_ITEM_ADVENTURER,
    TABLE_ITEM_BEATING,
    NUMBER_OF_TABLE_ITEMS
}table_items_t;

namespace Ui {
class Window_MainBoard;
}

class Window_MainBoard : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor
    explicit Window_MainBoard(QWidget *parent = nullptr);

    // Public methods
    void allocatePlayers(unsigned int playerCount);
    void addPlayer(unsigned int ID, QString name, QString character);
    void allocateCities(unsigned int cityCount);
    void setDayCount(unsigned int dayCount);

    // Destructor
    ~Window_MainBoard();

signals:
    void userActionDialogOpened(void);
    void newPlayerAndCityGameEngineData(const std::vector<Player>& playerList, const std::vector<City>& CityList); // For UserActionDialog only
    void newTempPlayerData(const Player& player, const Player& playerTemp); // For UserActionDialog only
    void newUserAction(Action userAction);
    void newTestUserAction(Action testAction);

public slots:
    void on_newPlayerData(const std::vector<Player>& playerList, const GameTimer& gameTimer);
    void on_newPlayerAndCityData(const std::vector<Player>& playerList, const std::vector<City>& cityList);
    void on_newTestUserAction(Action testAction);
    void on_newTempPlayerData(const Player& player, const Player& playerTemp);

private slots:
    // Pushbutton slots
    void on_pushButton_enterUserAction_clicked();
    void on_pushButton_cityDiscovered_clicked();
    void on_pushButton_playPauseGame_clicked();
    void on_pushButton_endGame_clicked();

    void on_pushButton_enterUserAction_pressed();
    void on_pushButton_enterUserAction_released();
    void on_pushButton_cityDiscovered_pressed();
    void on_pushButton_cityDiscovered_released();
    void on_pushButton_playPauseGame_pressed();
    void on_pushButton_playPauseGame_released();
    void on_pushButton_endGame_pressed();
    void on_pushButton_endGame_released();

private:
    // Private methods
    void updatePlayerStats(const std::vector<Player>& playerList);
    void setPlayerGraphics(const std::vector<Player>& playerList);
    void updateCityStats(Action action);
    void updatePlayerRanks(void);
    void updateCityList(void);
    void updateGreatestTitleList(void);
    void updateGreatestTitleStats(void);
    void playerTableSetupClean(void);
    void cityTableSetupClean(void);
    void greatestTitleTableSetupClean(void);
    void graphWidgetSetupClean(void);
    void findCityLeader(unsigned int cityID);
    //void findGreatestTitles(void);

    // Private members
    Ui::Window_MainBoard *ui;
    Dialog_UserAction *actionDialog;

    GameTimer *mpTimer = new GameTimer();
    unsigned long mGraphTimerMaxRange = 60;
    unsigned int mGraphPointMaxRange = 25;
    unsigned int mDayCount = 0;

    std::vector<Player> mPlayerListCopy; // Copy of data-list in GameEngine
    std::vector<City> mCityListCopy;     // Copy of data-list in GameEngine

    std::vector<Player> MageKnight_Player; // List of player objects (resized at initialization)
    std::vector<City> MageKnight_Cities;   // List of discovered cities on the map (resized at initialization)
    std::vector<unsigned int> PlayerRank;  // List of player ranks for run-time reordering of the players in the table
    //std::vector <QString> greatestTitlesPlayers;
    std::vector <QString> greatestTitles;

    QString mStyle_PushButton_normal   = " QPushButton {border-image: url(:/images/Image_PushButton_Normal);   font: 75 italic 14pt 'Cambria'; color: rgb(72, 52, 24) }";
    QString mStyle_PushButton_pressed  = " QPushButton {border-image: url(:/images/Image_PushButton_Pressed);  font: 75 italic 14pt 'Cambria'; color: rgb(72, 52, 24) }";
    QString mStyle_PushButton_disabled = " QPushButton {border-image: url(:/images/Image_PushButton_Disabled); font: 75 italic 14pt 'Cambria'; color: rgb(72, 52, 24) }";
};

#endif // Window_MainBoard_H
