#ifndef ACTION_H
#define ACTION_H

#include <QStringList>
#include <vector>
#include "Monster.h"
#include "unit.h"

typedef enum action_id_t
{
    ACTION_ID_TEXT,
    ACTION_ID_BURNED_MONASTERY,
    ACTION_ID_CITY,
    ACTION_ID_DUNGEON,
    ACTION_ID_INTERACTION,
    ACTION_ID_KEEP,
    ACTION_ID_LABYRINTH,
    ACTION_ID_MAGE_TOWER,
    ACTION_ID_MAZE,
    ACTION_ID_MONSTER_DEN,
    ACTION_ID_PLUNDERED_VILLAGE,
    ACTION_ID_RAMPAGING_MONSTER,
    ACTION_ID_RUINS,
    ACTION_ID_SPAWNING_GROUNDS,
    ACTION_ID_TOMB,
    NUMBER_OF_ACTION_IDS
}action_id_t;

typedef enum optional_dungeon_t
{
    OPTIONAL_DUNGEON_TEXT,
    OPTIONAL_DUNGEON_SPELL,
    OPTIONAL_DUNGEON_ARTIFACT,
    NUMBER_OF_OPTIONAL_DUNGEON_IDS
}optional_dungeon_t;

typedef enum optional_labyrinth_t
{
    OPTIONAL_LABYRINTH_TEXT,
    OPTIONAL_LABYRINTH_1_AAC_2_CRYSTALS,
    OPTIONAL_LABYRINTH_1_AAC_1_SPELL,
    OPTIONAL_LABYRINTH_1_AAC_1_ARTIFACT,
    NUMBER_OF_OPTIONAL_LABYRINTH_IDS
}optional_labyrinth_t;

typedef enum optional_maze_t
{
    OPTIONAL_MAZE_TEXT,
    OPTIONAL_MAZE_2_CRYSTALS,
    OPTIONAL_MAZE_1_SPELL,
    OPTIONAL_MAZE_1_ARTIFACT,
    NUMBER_OF_OPTIONAL_MAZE_IDS
}optional_maze_t;

typedef enum optional_monster_den_t
{
    OPTIONAL_MONSTER_DEN_TEXT,
    OPTIONAL_MONSTER_DEN_2_CRYSTALS,
    OPTIONAL_MONSTER_DEN_1_CRYSTAL_1_FAME,
    OPTIONAL_MONSTER_DEN_2_FAME,
    NUMBER_OF_OPTIONAL_MONSTER_DEN_IDS
}optional_monster_den_t;

typedef enum optional_ruin_t
{
    OPTIONAL_RUIN_TEXT,
    OPTIONAL_RUIN_7_FAME,
    OPTIONAL_RUIN_10_FAME,
    OPTIONAL_RUIN_1_ARTIFACT,
    OPTIONAL_RUIN_2_ARTIFACTS,
    OPTIONAL_RUIN_1_ARTIFACT_1_AAC,
    OPTIONAL_RUIN_1_ARTIFACT_1_SPELL,
    OPTIONAL_RUIN_1_SPELL_CRYSTALS,
    OPTIONAL_RUIN_CRYSTALS,
    OPTIONAL_RUIN_UNIT,
    NUMBER_OF_OPTIONAL_RUIN_IDS
}optional_ruin_t;

typedef enum optional_spawning_grounds_t
{
    OPTIONAL_SPAWNING_GROUNDS_TEXT,
    OPTIONAL_SPAWNING_GROUNDS_ARTIFACT_3_CRYSTALS,
    OPTIONAL_SPAWNING_GROUNDS_ARTIFACT_2_CRYSTALS_1_FAME,
    OPTIONAL_SPAWNING_GROUNDS_ARTIFACT_1_CRYSTALS_2_FAME,
    OPTIONAL_SPAWNING_GROUNDS_ARTIFACT_3_FAME,
    NUMBER_OF_OPTIONAL_SPAWNING_GROUNDS_IDS
}optional_spawning_grounds_t;


class Action
{
public:
    // Constructor
    Action();

    // Public members
    unsigned int mPlayerID = 0;
    unsigned int mMainActionID = 0;
    unsigned int mOptionalID = 0;
    bool mSuccessFail = false;
    int mWounds = 0;
    unsigned int mTime = 0;
    unsigned int mCityID = 0;

    // Additional values (besides from the main action)
    unsigned int mAddFame = 0;
    int mAddRepStep = 0;
    unsigned int mAddSpells = 0;
    unsigned int mAddAAC = 0;
    unsigned int mCrystals = 0;


    // Thrown/discarded values
    unsigned int mThrownArtifacts = 0;
    unsigned int mThrownAAC = 0;

    std::vector<Monster> mMonsters;
    std::vector<Unit> mUnits;

    // Public methods
    void reset();
    void addMonster(unsigned int MonsterCount);
    void removeMonster(unsigned int MonsterCount);
    void addUnit(unsigned int UnitCount);
    void removeUnit(unsigned int UnitCount);

private:
    // Private members
};

#endif // ACTION_H
