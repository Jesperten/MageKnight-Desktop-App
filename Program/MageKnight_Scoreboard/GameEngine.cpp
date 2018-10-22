#include "GameEngine.h"

// Constructor
GameEngine::GameEngine()
{
    // Clear the arrays by resizing to 0
    mMageKnight_Player.resize(0);
    mMageKnight_Cities.resize(0);
}

// Destructor
GameEngine::~GameEngine()
{

}

// Public Slots
void GameEngine::on_mageKnightPlayerAdded(QString playerName, QString playerCharacter) {
    Player newPlayer;
    newPlayer.setName(playerName);
    newPlayer.setCharacter(playerCharacter);

    unsigned int nPlayers = unsigned(mMageKnight_Player.size());
    newPlayer.mId = nPlayers+1;
    mMageKnight_Player.resize(nPlayers+1);
    mMageKnight_Player.at(nPlayers) = newPlayer;

    emit newMageKnightPlayerListReady(mMageKnight_Player); // It is important that the array is send as a read-only (const) pointer to improve speed (avoid copying data)
}
