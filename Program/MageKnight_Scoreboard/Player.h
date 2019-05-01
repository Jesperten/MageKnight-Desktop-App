#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QVector>
#include <QColor>
#include "Monster.h"
#include "Unit.h"

typedef enum greatest_title_items_t {
    TITLE_GREATEST,
    TITLE_GREATEST_KNOWLEDGE,
    TITLE_GREATEST_LOOT,
    TITLE_GREATEST_LEADER,
    TITLE_GREATEST_CONQUEROR,
    TITLE_GREATEST_CITY_CONQUEROR,
    TITLE_GREATEST_ADVENTURER,
    TITLE_GREATEST_BEATING,
    NUMBER_OF_GREATEST_TITLES
}greatest_title_items_t;

typedef enum city_relation_t {
    CITY_RELATION_NONE,
    CITY_RELATION_HAS_TOKENS,
    CITY_RELATION_IS_TIED_LEADER,
    CITY_RELATION_IS_LEADER
}city_relation_t;

typedef enum greatest_title_t {
    GREATEST_TITLE_NONE,
    GREATEST_TITLE_WIN_TIED,
    GREATEST_TITLE_WIN,
    NUMBER_OF_GREATEST_STATES
}greatest_title_t;


class Player {

public:
    // Constructor
    Player();

    // Public methods
    void addMonster(Monster monster);
    void addUnit(Unit unit);
    void addNewCity(void);
    void setGreatestTitleState(greatest_title_items_t item, greatest_title_t state, int score);

    // Public members
          QString mName = "";           // Player name (given at the player entrance)
    //    QString mTitle = "";          // Player title (not used yet)
          QString mCharacter = "";      // Player character (given at the player entrance)
          QColor mPlayerColor;          // Player color (given at the player entrance)
    unsigned int mRank = 0   ;          // Over-all game ranking
    unsigned int mId = 0;               // Player ID
             int mScore = 0;            // Total score (what actual matters and determines the rankings)
    unsigned int mLevel = 1;            // The player level starts at level 1
    unsigned int mFame = 0;             // Player fame (increases when killing monsters etc.)
             int mReputation = 0;       // Player reputation (starts at 0) - can be both positive (good influence) and negative (bad influence)
    unsigned int mRepStep = 7;          // Reputation step is used to keep track of the reputation, as multiple steps has the same reputation level. The Initial Step is at the middle (Rep value = 0)
    unsigned int mAActionCards = 0;     // Number of Advanced Action Cards (AAC)
    unsigned int mSpellCards = 0;       // Number of Spell Cards
    unsigned int mArtifacts = 0;        // Number of Artifacts
    unsigned int mCrystals = 0;         // Number of Crystals
    unsigned int mWounds = 0;           // Number of Wounds
    unsigned int mAdventureSites = 0;   // Adventure sites successfully visted
    unsigned int mConquerings = 0;      // Number of successful Conquerings
    unsigned int mCityTokensTotal = 0;  // Total number of City Tokens (achieved when successfully attacking cities)

    // ----------------------------------------------- Vector based Player data
    std::vector<int> mBasicScoreValues;                // The individual scores used for finding the end-game greatest titles (Knowledge, loot, leader etc.)
    std::vector<Unit> mUnits;                          // A vector of all the hired player units (dynamically reshapen at every action)
    std::vector<Monster> mMonsters;                    // A vector of all the slain monsters
    std::vector<unsigned int> mCityTokens;             // A vector of the number of city tokens (each entrance is a different city)
    std::vector<city_relation_t> mCityRelations;       // A vector of the relation levels of each city (each entrance is a different city)
    std::vector<greatest_title_t> mGreatestTitleStats; // The state of each Greatest Title (each entrance is a different title)
    std::vector<int> mGreatestTitleScores;             // The additional score gained from each relevant Greatest Title (each entrance is a different title)

    QVector<double> mPointData; // Point data array for the graphical plot (and score history)
    QVector<double> mTimeData;  // Corresponding time data array for the graphical plot (and score history)
};

#endif // PLAYER_H
