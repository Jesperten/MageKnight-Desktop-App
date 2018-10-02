#include "Player.h"

Player::Player()
{
    // Constructor of the Player class

    mName = "Volkare Biddybob";
    setTitle(); // Create the title for the player (generated at random)
    mGreatestTitleScores.resize(NUMBER_OF_GREATEST_TITLES);
    mGreatestScoreValues.resize(NUMBER_OF_GREATEST_TITLES);
    mGreatestTitleStats.resize(NUMBER_OF_GREATEST_TITLES);

    greatestTitlesScoreSetupClean();

    mPointData.resize(2);
    mTimeData.resize(2);

    mPointData = {0,0};
    mTimeData = {0,0};
}

/** *****************
  * PUBLIC METHODS *
  *******************/

void Player::setName(QString name)
{
    mName = name;
}

void Player::setCharacter(QString character)
{
    mCharacter = character;
}

void Player::addAction(Action playerAction)
{
    int tempRepStep = int(mRepStep);

    switch (playerAction.mMainActionID) {

    case(ACTION_ID_MAGE_TOWER):
        tempRepStep -= 1;

        if (playerAction.mSuccessFail == true)
        {
            mConquerings++;
            mSpellCards += 1;
        }

        break;

    case(ACTION_ID_KEEP):
        tempRepStep -= 1;

        if (playerAction.mSuccessFail == true)
        {
            mConquerings++;
        }

        break;

    case(ACTION_ID_CITY):
        tempRepStep -= 1;
        break;

    case(ACTION_ID_DUNGEON):

        if (playerAction.mSuccessFail == true)
        {
            mAdventureSites++;
            setOptionalDungeonBonus(playerAction.mOptionalID);
        }

        break;

    case(ACTION_ID_TOMB):

        if (playerAction.mSuccessFail == true)
        {
            mAdventureSites++;
            mSpellCards++;
            mArtifacts++;
        }

        break;

    case(ACTION_ID_MONSTER_DEN):

        if (playerAction.mSuccessFail == true)
        {
            mAdventureSites++;
            setOptionalMonsterDenBonus(playerAction.mOptionalID);
        }

        break;

    case(ACTION_ID_SPAWNING_GROUNDS):

        if (playerAction.mSuccessFail == true)
        {
            mAdventureSites++;
            setOptionalSpawningGroundsBonus(playerAction.mOptionalID);
        }

        break;

    case(ACTION_ID_RUINS):

        if (playerAction.mSuccessFail == true)
        {
            mAdventureSites++;
            setOptionalRuinBonus(playerAction.mOptionalID);
        }

        break;

    case(ACTION_ID_MAZE):

        if (playerAction.mSuccessFail == true)
        {
            mAdventureSites++;
            setOptionalMazeBonus(playerAction.mOptionalID);
        }

        break;

    case(ACTION_ID_LABYRINTH):

        if (playerAction.mSuccessFail == true)
        {
            mAdventureSites++;
            setOptionalLabyrinthBonus(playerAction.mOptionalID);
        }

        break;

    case(ACTION_ID_BURNED_MONASTERY):
        tempRepStep -= 3;

        if (playerAction.mSuccessFail == true)
        {
            mConquerings++;
            mArtifacts += 1;
        }

        break;

    case(ACTION_ID_PLUNDERED_VILLAGE):
        tempRepStep -= 1;
        break;

    default:
        break;
    }

    mFame += playerAction.mAddFame;
    mRepStep = unsigned( int(mRepStep) + playerAction.mAddRepStep);
    mAActionCards += playerAction.mAddAAC;
    mSpellCards += playerAction.mAddSpells;
    mCrystals += playerAction.mCrystals;
    mWounds = unsigned( int(mWounds) + playerAction.mWounds);

    mAActionCards -= playerAction.mThrownAAC;
    mArtifacts -= playerAction.mThrownArtifacts;

    for (unsigned int i = 0; i < playerAction.mMonsters.size(); ++i)
    {
        addMonster(playerAction.mMonsters.at(i));

        mFame += playerAction.mMonsters.at(i).mFame;

        if (mMonsters.at(i).mSpecial == "Hero")
        {
            tempRepStep -= 1;
            mFame += 1;
        }
        else if (mMonsters.at(i).mSpecial == "Faction 1")
        {
            tempRepStep += 1;
        }
        else if (mMonsters.at(i).mSpecial == "Faction 2")
        {
            tempRepStep += 1;
        }

        if (playerAction.mMonsters.at(i).mRampaging)
        {
            if      (playerAction.mMonsters.at(i).mType == "Orc Marauders") tempRepStep += 1;
            else if (playerAction.mMonsters.at(i).mType == "Draconum")      tempRepStep += 2;
        }
    }

    setLevel(fameToLevel(mFame));

         if (tempRepStep < 0)  mRepStep = 0;
    else if (tempRepStep > 14) mRepStep = 14;
    else                       mRepStep = unsigned(tempRepStep);

    mReputation = repStepToRep(mRepStep);

    // Clear Unit vector before (re)adding the units reported in the action
    // This includes all old and new units with updated wound states.
    mUnits.resize(0);

    for (unsigned int i = 0; i < playerAction.mUnits.size(); ++i)
    {
        addUnit(playerAction.mUnits.at(i));
    }

    updateScore();
}

void Player::setNumberOfCities(unsigned int cities)
{
    mCityTokens.resize(cities);
    mCityRelations.resize(cities);

    for (unsigned int i = 0; i < cities; ++i)
    {
        mCityRelations.at(i) = CITY_RELATION_NONE;
    }
}

void Player::setGreatestTitleState(greatest_title_items_t item, greatest_title_t state)
{
    // Find the value in the defined matrix, to set the corresponding score.
    mGreatestTitleScores.at(item) = mGreatestValues.at(item).at(state);
    mGreatestTitleStats.at(item) = state;
}

/** *****************
  * PRIVATE METHODS *
  *******************/

void Player::updateScore()
{
    mKnowledgePoints = (mSpellCards * SCORE_PER_SPELL) + (mAActionCards * SCORE_PER_AAC);
    mLootPoints = (mArtifacts * SCORE_PER_ARTIFACT) + (mCrystals/2 * SCORE_PER_TWO_CRYSTALS);
    mConquerorPoints = mConquerings * SCORE_PER_CONQUERING;
    mAdventurerPoints = mAdventureSites * SCORE_PER_ADVENTURE_SITE;
    mBeatingPoints = int(mWounds) * SCORE_PER_WOUND;

    mLeaderPoints = getTotalUnitPoints();
    mCityPoints = getCityPoints();

    // Update the values used in the Window_MainBoard to find the Greatest Title owners
    mGreatestScoreValues.at(TABLE_ITEM_GREATEST_KNOWLEDGE)      = mKnowledgePoints;
    mGreatestScoreValues.at(TABLE_ITEM_GREATEST_LOOT)           = mLootPoints;
    mGreatestScoreValues.at(TABLE_ITEM_GREATEST_LEADER)         = mLeaderPoints;
    mGreatestScoreValues.at(TABLE_ITEM_GREATEST_CONQUEROR)      = mConquerorPoints;
    mGreatestScoreValues.at(TABLE_ITEM_GREATEST_CITY_CONQUEROR) = mCityPoints;
    mGreatestScoreValues.at(TABLE_ITEM_GREATEST_ADVENTURER)     = mAdventurerPoints;
    mGreatestScoreValues.at(TABLE_ITEM_GREATEST_BEATING)        = mWounds;

    // Recalculate the complete scores for the player
    int score = 0;

    score += mFame;
    score += mKnowledgePoints;
    score += mLootPoints;
    score += mLeaderPoints;
    score += mConquerorPoints;
    score += mAdventurerPoints;
    score += mBeatingPoints;
    score += mCityPoints;
    score += getGreatestTitlesScores();

    mScore = score;
}

void Player::setTitle()
{
    mTitle = "Random title goes here";
}

void Player::setLevel(unsigned int level)
{
    unsigned int deltaLevel = level - mLevel;

    // Check if the level has increased
    if (deltaLevel > 0)
    {
        // This loop is done to enable multiple added AAC in case of multi-level increase
        for (unsigned int i = 1; i <= deltaLevel; ++i)
        {
            if ((mLevel + i) % 2 == 0) mAActionCards += 1; // Gain an advanced action card at each even level (2, 4, 6, 8 etc.)
        }

        mLevel = level;
    }
}

void Player::addMonster(Monster monster)
{
    unsigned int N = mMonsters.size();
    mMonsters.resize(N+1);
    mMonsters.at(N) = monster;
}

void Player::addUnit(Unit unit)
{
    unsigned int N = mUnits.size();
    mUnits.resize(N+1);
    mUnits.at(N) = unit;
}

unsigned int Player::getTotalUnitPoints(void)
{
    unsigned int tempScore = 0;

    for (unsigned int i = 0; i < mUnits.size(); ++i)
    {
        tempScore += mUnits.at(i).getScore();
    }

    return tempScore * SCORE_PER_UNIT_LEVEL;
}

unsigned int Player::getCityPoints(void)
{
    unsigned int cityScore = 0;

    for (unsigned int i = 0; i < mCityRelations.size(); ++i)
    {
        switch (mCityRelations.at(i))
        {
        case CITY_RELATION_NONE:
            break;

        case CITY_RELATION_HAS_TOKENS:
            cityScore += SCORE_PER_CITY_PARTITION;
            break;

        case CITY_RELATION_IS_TIED_LEADER:
            cityScore += SCORE_PER_CITY_LEADERSHIP_TIED;
            break;

        case CITY_RELATION_IS_LEADER:
            cityScore += SCORE_PER_CITY_LEADERSHIP;
            break;
        }
    }

    return cityScore;
}

void Player::greatestTitlesScoreSetupClean(void)
{
    // Greatest title score definitions are setup in a matrix.

    // Resize the matrix to match the "number of titles" x "number of state per title" (7x3)
    mGreatestValues.resize(NUMBER_OF_GREATEST_TITLES);

    for (unsigned int i = 0; i < NUMBER_OF_GREATEST_TITLES; ++i)
    {
        mGreatestValues.at(i).resize(NUMBER_OF_GREATEST_STATES);
    }

    // Define the values in each element manually.
    // ---------------------------------------------------------------------------------------- GREATEST KNOWLEDGE
    mGreatestValues.at(TABLE_ITEM_GREATEST_KNOWLEDGE).at(GREATEST_TITLE_NONE)          =  0; // NONE
    mGreatestValues.at(TABLE_ITEM_GREATEST_KNOWLEDGE).at(GREATEST_TITLE_WIN_TIED)      =  1; // WIN (TIED)
    mGreatestValues.at(TABLE_ITEM_GREATEST_KNOWLEDGE).at(GREATEST_TITLE_WIN)           =  3; // WIN

    // ---------------------------------------------------------------------------------------- GREATEST LOOT
    mGreatestValues.at(TABLE_ITEM_GREATEST_LOOT).at(GREATEST_TITLE_NONE)               =  0; // NONE
    mGreatestValues.at(TABLE_ITEM_GREATEST_LOOT).at(GREATEST_TITLE_WIN_TIED)           =  1; // WIN (TIED)
    mGreatestValues.at(TABLE_ITEM_GREATEST_LOOT).at(GREATEST_TITLE_WIN)                =  3; // WIN

    // ---------------------------------------------------------------------------------------- GREATEST LEADER
    mGreatestValues.at(TABLE_ITEM_GREATEST_LEADER).at(GREATEST_TITLE_NONE)             =  0; // NONE
    mGreatestValues.at(TABLE_ITEM_GREATEST_LEADER).at(GREATEST_TITLE_WIN_TIED)         =  1; // WIN (TIED)
    mGreatestValues.at(TABLE_ITEM_GREATEST_LEADER).at(GREATEST_TITLE_WIN)              =  3; // WIN

    // ---------------------------------------------------------------------------------------- GREATEST CONQUEROR
    mGreatestValues.at(TABLE_ITEM_GREATEST_CONQUEROR).at(GREATEST_TITLE_NONE)          =  0; // NONE
    mGreatestValues.at(TABLE_ITEM_GREATEST_CONQUEROR).at(GREATEST_TITLE_WIN_TIED)      =  1; // WIN (TIED)
    mGreatestValues.at(TABLE_ITEM_GREATEST_CONQUEROR).at(GREATEST_TITLE_WIN)           =  3; // WIN

    // ---------------------------------------------------------------------------------------- GREATEST CITY CONQUEROR
    mGreatestValues.at(TABLE_ITEM_GREATEST_CITY_CONQUEROR).at(GREATEST_TITLE_NONE)     =  0; // NONE
    mGreatestValues.at(TABLE_ITEM_GREATEST_CITY_CONQUEROR).at(GREATEST_TITLE_WIN_TIED) =  2; // WIN (TIED)
    mGreatestValues.at(TABLE_ITEM_GREATEST_CITY_CONQUEROR).at(GREATEST_TITLE_WIN)      =  5; // WIN

    // ---------------------------------------------------------------------------------------- GREATEST ADVENTURER
    mGreatestValues.at(TABLE_ITEM_GREATEST_ADVENTURER).at(GREATEST_TITLE_NONE)         =  0; // NONE
    mGreatestValues.at(TABLE_ITEM_GREATEST_ADVENTURER).at(GREATEST_TITLE_WIN_TIED)     =  1; // WIN (TIED)
    mGreatestValues.at(TABLE_ITEM_GREATEST_ADVENTURER).at(GREATEST_TITLE_WIN)          =  3; // WIN

    // ---------------------------------------------------------------------------------------- GREATEST BEATING
    mGreatestValues.at(TABLE_ITEM_GREATEST_BEATING).at(GREATEST_TITLE_NONE)            =  0; // NONE
    mGreatestValues.at(TABLE_ITEM_GREATEST_BEATING).at(GREATEST_TITLE_WIN_TIED)        = -1; // WIN (TIED)
    mGreatestValues.at(TABLE_ITEM_GREATEST_BEATING).at(GREATEST_TITLE_WIN)             = -3; // WIN
}

void Player::setOptionalDungeonBonus(unsigned optionalID)
{
    switch(optionalID)
    {
    case (OPTIONAL_DUNGEON_SPELL): // "+1 Spell Card"
        mSpellCards += 1;
        break;

    case OPTIONAL_DUNGEON_ARTIFACT: // "+1 Artifact"
        mArtifacts += 1;
        break;

    default:
        break;
    }
}

void Player::setOptionalLabyrinthBonus(unsigned optionalID)
{
    mAActionCards += 1;

    switch(optionalID)
    {
    case (OPTIONAL_LABYRINTH_1_AAC_2_CRYSTALS): // "+2 Crystals"
        mCrystals += 2;
        break;

    case OPTIONAL_LABYRINTH_1_AAC_1_SPELL: // "+1 Spell Card"
        mSpellCards += 1;
        break;

    case OPTIONAL_LABYRINTH_1_AAC_1_ARTIFACT: // "+1 Artifact"
        mArtifacts += 1;
        break;

    default:
        break;
    }
}

void Player::setOptionalMazeBonus(unsigned optionalID)
{
    switch(optionalID)
    {
    case (OPTIONAL_MAZE_2_CRYSTALS): // "+2 Crystals"
        mCrystals += 2;
        break;

    case OPTIONAL_MAZE_1_SPELL: // "+1 Spell Card"
        mSpellCards += 1;
        break;

    case OPTIONAL_MAZE_1_ARTIFACT: // "+1 Artifact"
        mArtifacts += 1;
        break;

    default:
        break;
    }
}

void Player::setOptionalMonsterDenBonus(unsigned optionalID)
{
    switch(optionalID)
    {
    case (OPTIONAL_MONSTER_DEN_2_CRYSTALS): // "+2 Crystals"
        mCrystals += 2;
        break;

    case OPTIONAL_MONSTER_DEN_1_CRYSTAL_1_FAME: // "+1 Crystal, +1 Fame"
        mCrystals += 1;
        mFame += 1;
        break;

    case OPTIONAL_MONSTER_DEN_2_FAME: // "+2 Fame"
        mFame += 2;
        break;

    default:
        break;
    }
}

void Player::setOptionalRuinBonus(unsigned optionalID)
{
    switch(optionalID)
    {
    case (OPTIONAL_RUIN_7_FAME): // "+7 Fame"
        mFame += 7;
        break;

    case OPTIONAL_RUIN_10_FAME: // "+10 Fame"
        mFame += 10;
        break;

    case OPTIONAL_RUIN_1_ARTIFACT: // "+1 Artifact"
        mArtifacts += 1;
        break;

    case OPTIONAL_RUIN_2_ARTIFACTS: // "+2 Artifacts"
        mArtifacts += 2;
        break;

    case OPTIONAL_RUIN_1_ARTIFACT_1_AAC: // "+1 Artifact, +1 AAC"
        mArtifacts += 1;
        mAActionCards += 1;
        break;

    case OPTIONAL_RUIN_1_ARTIFACT_1_SPELL: // "+1 Artifact, +1 Spell"
        mArtifacts += 1;
        mSpellCards += 1;
        break;

    case OPTIONAL_RUIN_1_SPELL_CRYSTALS: // "+1 Spell, +Crystals"
        mSpellCards += 1;
        mCrystals += 3;
        break;

    case OPTIONAL_RUIN_CRYSTALS: // "+Crystals"
        mCrystals += 3;
        break;

    default:
        break;
    }
}

void Player::setOptionalSpawningGroundsBonus(unsigned optionalID)
{
    mArtifacts += 1;

    switch(optionalID)
    {
    case (OPTIONAL_SPAWNING_GROUNDS_ARTIFACT_3_CRYSTALS): // "+1 Artifact, +3 Crystals"
        mCrystals += 3;
        break;

    case OPTIONAL_SPAWNING_GROUNDS_ARTIFACT_2_CRYSTALS_1_FAME: // "+1 Artifact, +2 Crystals, +1 Fame"
        mCrystals += 2;
        mFame += 1;
        break;

    case OPTIONAL_SPAWNING_GROUNDS_ARTIFACT_1_CRYSTALS_2_FAME: // "+1 Artifact, +1 Crystals, 2 Fame"
        mCrystals += 1;
        mFame += 3;
        break;

    case OPTIONAL_SPAWNING_GROUNDS_ARTIFACT_3_FAME: // "+1 Artifact, +3 Fame"
        mFame += 3;
        break;

    default:
        break;
    }
}

int Player::getGreatestTitlesScores(void)
{
    int titlesScore = 0;

    for (unsigned int i = 0; i < mGreatestTitleScores.size(); ++i)
    {
        titlesScore += mGreatestTitleScores.at(i);
    }

    return titlesScore;
}

unsigned int Player::fameToLevel(unsigned int fame)
{
    if      (fame > 98) return 10;
    else if (fame > 79) return 9;
    else if (fame > 62) return 8;
    else if (fame > 47) return 7;
    else if (fame > 34) return 6;
    else if (fame > 23) return 5;
    else if (fame > 14) return 4;
    else if (fame > 7)  return 3;
    else if (fame > 2)  return 2;
    else                return 1;
}

int Player::repStepToRep(unsigned int repStep)
{
    // Repstep                 0,  1,  2,  3,  4,  5, 6, 7, 8, 9,10,11,12,13,14
    std::vector<int> rep = {-100, -5, -3, -2, -1, -1, 0, 0, 0, 1, 1, 2, 2, 3, 5};

   if (repStep > 14) repStep = 14;

    return rep.at(repStep);
}
