#ifndef MONSTER_H
#define MONSTER_H

#include<QString>

typedef enum monster_type_t
{
    MONSTER_TYPE_TEXT,
    MONSTER_TYPE_MARAUDING_ORC,
    MONSTER_TYPE_KEEP,
    MONSTER_TYPE_MAGE_TOWER,
    MONSTER_TYPE_DUNGEON,
    MONSTER_TYPE_CITY,
    MONSTER_TYPE_DRACONUM,
    MONSTER_TYPE_RUINS,
    NUMBER_OF_MONSTER_TYPES
}monster_type_t;

class Monster
{
public:
    // Constructor
    Monster();

    // Public members
    QString mType = "";
    QString mSpecial = "";
    unsigned int mFame = 0;
    bool mRampaging = false;
};

#endif // MONSTER_H
