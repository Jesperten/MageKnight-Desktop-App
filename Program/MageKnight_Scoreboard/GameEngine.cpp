#include "GameEngine.h"
#include "GameScoreSystem.h"
#include <math.h>

// Constructor
GameEngine::GameEngine() {
    // Clear the arrays by resizing to 0
    mMageKnight_Player.resize(0);
    mMageKnight_Cities.resize(0);
    mGameTimer.setTimer(0);
    mGameTimer.stopTimer();

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
    mMageKnight_Player.resize(nPlayers);
    mMageKnight_Player.at(nPlayers-1) = newPlayer;

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

    emit newMageKnightPlayerListReady(mMageKnight_Player); // It is important that the array is send as a read-only (const) reference to improve speed (avoid copying data)
}

void GameEngine::on_newGameStarted() {
    mGameTimer.startTimer();
    QObject::connect(&mGameTimer, SIGNAL(gameTimerUpdate()), this, SLOT(on_timerUpdate()));

    emit newMageKnightPlayerData(mMageKnight_Player, mGameTimer);
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

    emit newMageKnightPlayerData(mMageKnight_Player, mGameTimer);
}

void GameEngine::on_userActionDialogOpened() {
    // This slot is meant as a trigger to send constant references to the playerList and cityList to the recently opened userActionDialog
    emit newPlayerAndCityData(mMageKnight_Player, mMageKnight_Cities);
}

void GameEngine::on_newUserAction(Action action) {
    // A user action has been completed and sent to the Game Engine.
    // The players and cities will be updated accordingly.

    addPlayerAction(action, &mMageKnight_Player.at(action.mPlayerID-1)); // Add the action and all its properties to the player
                                                                         // Update points from cities across all players
    updateBasicScores(&mMageKnight_Player);                              // Update the basic scores on all players
    findGreatestTitles(&mMageKnight_Player);                             // Update Greatest titles across all players
    updateFinalScores(&mMageKnight_Player);                              // Update the final scores across all players
    findRanks(&mMageKnight_Player);                                      // Update ranks across all players
    emit newMageKnightPlayerData(mMageKnight_Player, mGameTimer);        // Needs to react on city data changes also...
}

void GameEngine::on_newTestUserAction(Action action) {
    if (action.mPlayerID > 0) { // Safety to filter out any calls during the GUI setup
        std::vector<Player> tempPlayerList = mMageKnight_Player; // Create a copy of the entire list of players in order to calculate temporary results on the action.

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


/*
void GameEngine::updateCityStats(Action action) {
    if (action.mMainActionID == ACTION_ID_CITY) {
        unsigned int cityID = action.mCityID;
        unsigned int playerID = action.mPlayerID - 1;
        unsigned int monsterCount = action.mMonsters.size();

        for (unsigned int i = 0; i < action.mMonsters.size(); ++i) {
            if (action.mMonsters.at(i).mRampaging) --monsterCount;
        }

        mMageKnight_Cities.at(cityID).mMonstersRemaining -= monsterCount; // Find the number of remaining monsters in the given city
        mMageKnight_Player.at(playerID).mCityTokens.at(cityID) += monsterCount; // Assign the number of tokens in the city for the given player

        if (mMageKnight_Cities.at(cityID).mMonstersRemaining == 0) {
            mMageKnight_Cities.at(cityID).mConquered = true;
            findCityLeader(cityID);
        }

        // updateCityList();
    }
}
*/
/*
void GameEngine::findCityLeader(unsigned int cityID) {
    unsigned int maxScore = 0;
    unsigned int playerScore = 0;
    bool tied = false;
    std::vector <unsigned int> cityLeaderIDs;
    QString cityOwner = "";

    if (mMageKnight_Cities.at(cityID).mConquered) {
        // Search through all players and determine the maxScore (number of tokens) and if it is tied between two or more players
        for (unsigned int i = 0; i< mMageKnight_Player.size(); ++i) {
            playerScore = mMageKnight_Player.at(i).mCityTokens.at(cityID);
            if (playerScore > maxScore) {
                maxScore = playerScore;
                tied = false;
            }
            else if (playerScore == maxScore)
                tied = true;
        }

        // Use the found maxscore to set the city relation status for all players
        for (unsigned int i = 0; i< mMageKnight_Player.size(); ++i) {
            playerScore = mMageKnight_Player.at(i).mCityTokens.at(cityID);

            if (playerScore == maxScore) {
                // Expand the list of city leader IDs by one
                unsigned int nLeaders = cityLeaderIDs.size();
                cityLeaderIDs.resize(nLeaders + 1);

                // Add the current playerID to the city leader IDs list
                cityLeaderIDs.at(nLeaders) = i;

                if (tied)
                    mMageKnight_Player.at(i).mCityRelations.at(cityID) = CITY_RELATION_IS_TIED_LEADER;
                else
                    mMageKnight_Player.at(i).mCityRelations.at(cityID) = CITY_RELATION_IS_LEADER;
            }

            else if (playerScore > 0)
                mMageKnight_Player.at(i).mCityRelations.at(cityID) = CITY_RELATION_HAS_TOKENS;
            else
                mMageKnight_Player.at(i).mCityRelations.at(cityID) = CITY_RELATION_NONE;

            mMageKnight_Player.at(i).updateScore();
        }

        if (tied) {
            cityOwner = mMageKnight_Player.at(cityLeaderIDs.at(0)).mName;

            for (unsigned int i = 1; i < cityLeaderIDs.size(); ++i) {
                cityOwner += ", ";
                cityOwner += mMageKnight_Player.at(cityLeaderIDs.at(i)).mName;
            }
            cityOwner += " (tied)";
        }
        else {
            cityOwner = mMageKnight_Player.at(cityLeaderIDs.at(0)).mName;
        }

        mMageKnight_Cities.at(cityID).mCityOwner = cityOwner;
    }
}
*/
