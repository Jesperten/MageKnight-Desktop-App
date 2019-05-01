#include "Player.h"

Player::Player() {
    // Constructor of the Player class

    mName = "";
    mGreatestTitleScores.resize(NUMBER_OF_GREATEST_TITLES);
    mBasicScoreValues.resize(NUMBER_OF_GREATEST_TITLES);
    mGreatestTitleStats.resize(NUMBER_OF_GREATEST_TITLES);

    mPointData.resize(2);
    mTimeData.resize(2);

    mCityTokens.resize(0);
    mCityRelations.resize(0);

    mPointData = {0,0};
    mTimeData = {0,0};
}

// ------------------ PUBLIC METHODS ------------------

void Player::addNewCity(void) {
    mCityTokens.push_back(0);
    mCityRelations.push_back(CITY_RELATION_NONE);
}

void Player::setGreatestTitleState(greatest_title_items_t item, greatest_title_t state, int score) {
    mGreatestTitleScores.at(item) = score;
    mGreatestTitleStats.at(item) = state;
}

// ------------------ PRIVATE METHODS ------------------

void Player::addMonster(Monster monster) {
    unsigned int N = mMonsters.size();
    mMonsters.resize(N+1);
    mMonsters.at(N) = monster;
}

void Player::addUnit(Unit unit) {
    unsigned int N = mUnits.size();
    mUnits.resize(N+1);
    mUnits.at(N) = unit;
}
