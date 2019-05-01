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

class Window_MainBoard : public QMainWindow {
    Q_OBJECT

public:
    // Constructor
    explicit Window_MainBoard(QWidget *parent = nullptr);

    // Destructor
    ~Window_MainBoard();

signals:
    void userActionDialogOpened(void);
    void playPauseToggle(void);

public slots:
    void on_newMageKnightData(const std::vector<Player>& playerList, const std::vector<City>& cityList, const std::vector<QString>& greatestTitles, const GameTimer& gameTimer);
    void on_gamePlayPauseState(bool gameActive);

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
    void updateCityList(const std::vector<City>& cityList);
    void updateGreatestTitleList(const std::vector<QString>& greatestTitles);
    void updateGreatestTitleStats(void);
    void playerTableSetupClean(void);
    void cityTableSetupClean(void);
    void greatestTitleTableSetupClean(void);
    void graphWidgetSetupClean(void);
    void findCityLeader(unsigned int cityID);

    // Private members
    Ui::Window_MainBoard *ui;

    unsigned long mGraphTimerMaxRange = 60;
    unsigned int mGraphPointMaxRange = 25;

    QString mStyle_PushButton_normal   = " QPushButton {border-image: url(:/images/Image_PushButton_Normal);   font: 75 italic 14pt 'Cambria'; color: rgb(72, 52, 24) }";
    QString mStyle_PushButton_pressed  = " QPushButton {border-image: url(:/images/Image_PushButton_Pressed);  font: 75 italic 14pt 'Cambria'; color: rgb(72, 52, 24) }";
    QString mStyle_PushButton_disabled = " QPushButton {border-image: url(:/images/Image_PushButton_Disabled); font: 75 italic 14pt 'Cambria'; color: rgb(72, 52, 24) }";
};

#endif // Window_MainBoard_H
