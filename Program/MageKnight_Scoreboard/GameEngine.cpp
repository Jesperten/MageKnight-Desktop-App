#include "GameEngine.h"
#include "GameScoreSystem.h"
#include "Dialog_UserAction.h"
#include <math.h>

GameEngine* GameEngine::instance_ = nullptr;

// Private Constructor (Singleton class)
GameEngine::GameEngine() {
    // Clear the arrays by resizing to 0
    mMageKnight_Player.resize(0);
    mMageKnight_Cities.resize(0);
    mGameTimer.setTimer(0);
    mGameTimer.stopTimer();

    mGreatestTitlesPlayers.resize(NUMBER_OF_GREATEST_TITLES);
    greatestTitlesScoreSetupClean();
}

// Destructor
GameEngine::~GameEngine() {

}

// Public Slots
void GameEngine::on_mageKnightPlayerAdded(QString playerName, QString playerCharacter) {
    Player newPlayer;
    newPlayer.mName = playerName;
    newPlayer.mCharacter = playerCharacter;

    unsigned int nPlayers = unsigned(mMageKnight_Player.size()) + 1; // The new number of added players is to be increased by one
    newPlayer.mId = nPlayers; // Todo: the index should start from 0 to make it easier for use with std::vector
    mMageKnight_Player.push_back(newPlayer);

    // Neat algorithm to find a new h-value such that the series gives equally and gradually finer
    // distributed integers from 0 to 360. (Works for all n, where 1 < n < 257, when n = 257 is the first duplicate value appears)
    // input value :          2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21
    // Output value = 180 * ( 1/1  1/2  3/2  1/4  3/4  5/4  7/4  1/8  3/8  5/8  7/8  9/8  11/8 13/8 15/8 1/16 3/16 5/16 7/16 9/16

    int h = 0;
    double temp;

    if (nPlayers > 1) {
        temp = pow(2.0, double(floor(log2(nPlayers-1))));
        h = int(round(180*(((nPlayers-int(temp))*2)-1)/temp));
    }

    mMageKnight_Player.at(nPlayers-1).mPlayerColor = QColor::fromHsv(h, 200, 200, 255);
    findRanks(&mMageKnight_Player); // Update the initial ranking of the players

    // todo: add all discovered cities to the new player (relevant only for late entrance players)

    emit newMageKnightPlayerListReady(mMageKnight_Player); // It is important that the array is send as a read-only (const) reference to improve speed (avoid copying data)
}

void GameEngine::on_newGameStarted() {
    mGameTimer.startTimer();
    QObject::connect(&mGameTimer, &GameTimer::gameTimerUpdate, this, &GameEngine::on_timerUpdate);

    emit newMageKnightData(mMageKnight_Player, mMageKnight_Cities, mGreatestTitlesPlayers, mGameTimer);
}

void GameEngine::on_gameReset() {
    mMageKnight_Player.resize(0);
    mMageKnight_Cities.resize(0);
    mGameTimer.setTimer(0);
    mGameTimer.stopTimer();
}

void GameEngine::on_timerUpdate() {
    // All player data graph representations need to be updated continuosly with the timer update.
    // This means that at every timer update, the last time value in each player data set must be
    // updated. Once these are updated, the new values are sent to the Main Board GUI.

    int endPoint = 0;

    for (unsigned int i = 0; i < mMageKnight_Player.size(); ++i)
    {
        endPoint = mMageKnight_Player.at(i).mTimeData.size();
        mMageKnight_Player.at(i).mTimeData[endPoint-1] = mGameTimer.mTicks;
    }

    emit newMageKnightData(mMageKnight_Player, mMageKnight_Cities, mGreatestTitlesPlayers, mGameTimer);
}

void GameEngine::on_userActionDialogOpened() {
    // This slot is meant as a trigger to send constant references to the playerList and cityList to the recently opened userActionDialog
    emit userActionDialogData(mMageKnight_Player, mMageKnight_Cities);
}

void GameEngine::on_newUserAction(Action action) {
    // A user action has been completed and sent to the Game Engine.
    // The players and cities will be updated accordingly.

    // In case that the reported action was a city attack, this most be handled specifically
    if ((action.mMainActionID == ACTION_ID_CITY) && (mMageKnight_Cities.at(action.mCityID).mConquered == false)) {

        updateCityStats(action, &mMageKnight_Cities.at(action.mCityID), &mMageKnight_Player.at(action.mPlayerID-1));

        if (mMageKnight_Cities.at(action.mCityID).mMonstersRemaining == 0) {
            mMageKnight_Cities.at(action.mCityID).mConquered = true;
            findCityLeader(&mMageKnight_Cities.at(action.mCityID), &mMageKnight_Player);
        }
    }

    addPlayerAction(action, &mMageKnight_Player.at(action.mPlayerID-1)); // Add the action and all its properties to the player
    updateBasicScores(&mMageKnight_Player);                              // Update the basic scores on all players
    findGreatestTitles(&mMageKnight_Player, &mGreatestTitlesPlayers);    // Update Greatest titles across all players
    updateFinalScores(&mMageKnight_Player);                              // Update the final scores across all players
    findRanks(&mMageKnight_Player);                                      // Update ranks across all players

    emit newMageKnightData(mMageKnight_Player, mMageKnight_Cities, mGreatestTitlesPlayers, mGameTimer);
}

void GameEngine::on_newTestUserAction(Action action) {
    // This slot is almost identical to "on_newUserAction", but is based on copies of the game data
    // in order to only find temporary results, without changing the actual gamedata

    if (action.mPlayerID > 0) { // Safety to filter out any calls during the GUI setup
        std::vector<Player> tempPlayerList = mMageKnight_Player; // Create a copy of the entire list of players in order to calculate temporary results on the action.
        std::vector<City> tempCityList = mMageKnight_Cities;     // Create a copy of the entire list of cities in order to calculate temporary results on the action.

        // In case that the reported action was a city attack, this most be handled specifically
        if ((action.mMainActionID == ACTION_ID_CITY) && (tempCityList.at(action.mCityID).mConquered == false)) {

            updateCityStats(action, &tempCityList.at(action.mCityID), &tempPlayerList.at(action.mPlayerID-1));

            if (tempCityList.at(action.mCityID).mMonstersRemaining == 0) {
                tempCityList.at(action.mCityID).mConquered = true;
                findCityLeader(&tempCityList.at(action.mCityID), &tempPlayerList);
            }
        }

        addPlayerAction(action, &tempPlayerList.at(action.mPlayerID-1));  // Add the action and all its properties to the player
                                                                          // Update points from cities across all players
        updateBasicScores(&tempPlayerList);                               // Update the basic scores on all players
        findGreatestTitles(&tempPlayerList);                              // Update Greatest titles across all players
        updateFinalScores(&tempPlayerList);                               // Update the final scores across all players
        findRanks(&tempPlayerList);                                       // Update ranks across all players

        // Send the temporaty and the original player for simpler comparison
        emit newTempPlayerData(mMageKnight_Player.at(action.mPlayerID-1), tempPlayerList.at(action.mPlayerID-1));
    }
}

void GameEngine::on_newCityDiscovered(City newCity) {
    mMageKnight_Cities.push_back(newCity);
    mMageKnight_Cities.back().mId = mMageKnight_Cities.size()-1;

    emit newMageKnightData(mMageKnight_Player, mMageKnight_Cities, mGreatestTitlesPlayers, mGameTimer);

    for(unsigned int i = 0; i < mMageKnight_Player.size(); i++) {
        mMageKnight_Player.at(i).addNewCity();
    }
}

void GameEngine::on_pausePlayToggle(void) {
    if (mGameTimer.isTimerActive()) {
        // Stop the timer and disable the "Enter User Action" button.
        mGameTimer.stopTimer();
    }
    else {
        // Restart the timer and enable the "Enter User Action" button.
        // The timer is restarted from 0 ms. Thus, up to 999 ms time tracking can be lost.
        mGameTimer.startTimer();
    }

    emit gamePlayPauseState(mGameTimer.isTimerActive());
}
