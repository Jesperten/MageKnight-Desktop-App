#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "City.h"
#include "Player.h"
#include "Action.h"
#include "GameTimer.h"
#include <QString>
#include <QObject>

class GameEngine : public QObject {
    Q_OBJECT

public:
    ~GameEngine();

    // Singleton instance reference getter
    const static GameEngine* instance() {
        if (instance_ == nullptr)
            instance_ = new GameEngine();
        return instance_;
    }

signals:
    // The mMageKnight_Player and _Cities must be send as read-only (const) pointers/references to improve speed and avoid data-access from other scopes.

    void newMageKnightData(const std::vector<Player>& playerList, const std::vector<City>& CityList, const std::vector<QString>& greatestTitles, const GameTimer& gameTimer); // For the MainWindow only

    void newMageKnightPlayerListReady(const std::vector<Player>& mageKnightPlayerList); // For startWindow only
    void userActionDialogData(const std::vector<Player>& playerList, const std::vector<City>& CityList); // For UserActionDialog only (init setup data for the GUI)
    void newTempPlayerData(const Player& player, const Player& playerTemp); // For UserActionDialog - temporary player data to see the expected results of the action being typed in
    void gamePlayPauseState(bool gameActive);

public slots:
    void on_mageKnightPlayerAdded(QString playerName, QString playerCharacter);
    void on_newGameStarted(bool Volkare_isPresent, City Volkare);
    void on_gameReset();
    void on_timerUpdate();
    void on_userActionDialogOpened();
    void on_newUserAction(Action action);
    void on_newTestUserAction(Action action);
    void on_newCityDiscovered(City newCity);
    void on_pausePlayToggle(void);

private:
    // Private methods
    GameEngine();

    // Private Members
    static GameEngine* instance_;
    std::vector<Player> mMageKnight_Player; // List of player objects
    std::vector<City> mMageKnight_Cities; // List of discovered cities on the map
    std::vector<QString> mGreatestTitlesPlayers; // List of strings used for display of greatest titles in the MainBoard window

    GameTimer mGameTimer;

    // Private constants
};

#endif // GAMEENGINE_H
