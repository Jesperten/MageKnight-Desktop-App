#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QVector>
#include <QColor>
#include "Action.h"

typedef enum greatest_title_items_t
{
    TABLE_ITEM_GREATEST_KNOWLEDGE,
    TABLE_ITEM_GREATEST_LOOT,
    TABLE_ITEM_GREATEST_LEADER,
    TABLE_ITEM_GREATEST_CONQUEROR,
    TABLE_ITEM_GREATEST_CITY_CONQUEROR,
    TABLE_ITEM_GREATEST_ADVENTURER,
    TABLE_ITEM_GREATEST_BEATING,
    NUMBER_OF_GREATEST_TITLES
}greatest_title_items_t;

typedef enum city_relation_t
{
    CITY_RELATION_NONE,
    CITY_RELATION_HAS_TOKENS,
    CITY_RELATION_IS_TIED_LEADER,
    CITY_RELATION_IS_LEADER
}city_relation_t;

typedef enum greatest_title_t
{
    GREATEST_TITLE_NONE,
    GREATEST_TITLE_WIN_TIED,
    GREATEST_TITLE_WIN,
    NUMBER_OF_GREATEST_STATES
}greatest_title_t;


class Player
{
public:
    // Constructor
    Player();

    // Public methods
    void updateScore();
    void setName(QString name);
    void setCharacter(QString character);
    void addAction(Action playerAction);
    void setNumberOfCities(unsigned int cities);
    void setGreatestTitleState(greatest_title_items_t item, greatest_title_t state);

    // Public members
             int mScore = 0;
    unsigned int mLevel = 1;
    unsigned int mFame = 0;
             int mReputation = 0;
    unsigned int mAActionCards = 0;
    unsigned int mSpellCards = 0;
    unsigned int mArtifacts = 0;
    unsigned int mCrystals = 0;
    unsigned int mWounds = 0;
    unsigned int mAdventureSites = 0;
    unsigned int mConquerings = 0;
    unsigned int mCityTokensTotal = 0;

    unsigned int mKnowledgePoints = 0;
    unsigned int mLootPoints = 0;
    unsigned int mLeaderPoints = 0;
    unsigned int mConquerorPoints = 0;
    unsigned int mAdventurerPoints = 0;
             int mBeatingPoints = 0;
    unsigned int mCityPoints = 0;

    QVector<double> mPointData;
    QVector<double> mTimeData;

    std::vector<Unit> mUnits;
    std::vector<Monster> mMonsters;
    std::vector<unsigned int> mCityTokens;
    std::vector<city_relation_t> mCityRelations;
    std::vector<unsigned int> mGreatestScoreValues;
    std::vector<greatest_title_t> mGreatestTitleStats;

    QColor mPlayerColor;

    QString mName = "";
    QString mTitle = "";
    QString mCharacter = "";

private:    
    // Private methods
    void setTitle(); // uses auto-generator
    void setLevel(unsigned int level);
    void addMonster(Monster monster);
    void addUnit(Unit unit);
    void greatestTitlesScoreSetupClean(void);
    void setOptionalDungeonBonus(unsigned optionalID);
    void setOptionalLabyrinthBonus(unsigned optionalID);
    void setOptionalMazeBonus(unsigned optionalID);
    void setOptionalMonsterDenBonus(unsigned optionalID);
    void setOptionalRuinBonus(unsigned optionalID);
    void setOptionalSpawningGroundsBonus(unsigned optionalID);

    unsigned int getTotalUnitPoints(void);
    unsigned int getCityPoints(void);
    int getGreatestTitlesScores(void);
    unsigned int fameToLevel(unsigned int fame);
    int repStepToRep(unsigned int repStep);

    // Private members
    unsigned int mRepStep = 7; // Initial RepStep is at the middle (Rep value = 0)

    std::vector<std::vector<int>> mGreatestValues;
    std::vector<int> mGreatestTitleScores;

    unsigned int SCORE_PER_AAC = 1;
    unsigned int SCORE_PER_SPELL = 2;
    unsigned int SCORE_PER_ARTIFACT = 2;
    unsigned int SCORE_PER_TWO_CRYSTALS = 1;
    unsigned int SCORE_PER_UNIT_LEVEL = 1;
    unsigned int SCORE_PER_CONQUERING = 2;     // Keep, Mage Tower and Monastery
    unsigned int SCORE_PER_ADVENTURE_SITE = 2; // Dungeon, Tomb, Monster Den, Spawning Grounds, Ruins
             int SCORE_PER_WOUND = -2;

    unsigned int SCORE_PER_CITY_TOKEN = 0;
    unsigned int SCORE_PER_CITY_PARTITION = 4;
    unsigned int SCORE_PER_CITY_LEADERSHIP_TIED = 4;
    unsigned int SCORE_PER_CITY_LEADERSHIP = 7;
};

#endif // PLAYER_H
