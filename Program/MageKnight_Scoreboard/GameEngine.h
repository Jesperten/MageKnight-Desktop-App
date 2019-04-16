#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "City.h"
#include "Player.h"
#include "Action.h"
#include "GameTimer.h"
#include <QString>
#include <QObject>

class GameEngine : public QObject
{
    Q_OBJECT

public:
    GameEngine();
   ~GameEngine();

signals:
    // The mMageKnight_Player and _Cities must be send as read-only (const) pointers/references to improve speed and avoid data-access from other scopes.
    void newMageKnightPlayerListReady(const std::vector<Player>& mageKnightPlayerList); // For startWindow only
    void newMageKnightPlayerData(const std::vector<Player>& playerList, const GameTimer& gameTimer); // For the MainWindow only
    void newTempPlayerData(const Player& player, const Player& playerTemp); // For UserActionDialog - temporary player data to see the expected results of the action being typed in
    void newPlayerAndCityData(const std::vector<Player>& playerList, const std::vector<City>& CityList); // For UserActionDialog only

public slots:
    void on_mageKnightPlayerAdded(QString playerName, QString playerCharacter);
    void on_newGameStarted();
    void on_gameReset();
    void on_timerUpdate();
    void on_userActionDialogOpened();
    void on_newUserAction(Action action);
    void on_newTestUserAction(Action action);

private:
    // Private methods

    // Private Members
    std::vector<Player> mMageKnight_Player; // List of player objects
    std::vector<City> mMageKnight_Cities; // List of discovered cities on the map
    GameTimer mGameTimer;

    // Private constants
};

#endif // GAMEENGINE_H
