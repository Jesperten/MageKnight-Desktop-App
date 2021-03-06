#ifndef UNIT_H
#define UNIT_H

#include <QString>

typedef enum unit_type_t
{
    UNIT_TYPE_TEXT,
    UNIT_TYPE_FORESTERS,
    UNIT_TYPE_HERBALISTS,
    UNIT_TYPE_PEASANTS,
    UNIT_TYPE_SCOUTS,
    UNIT_TYPE_THUGS,
    UNIT_TYPE_GUARDIAN_GOLEMS,
    UNIT_TYPE_ILLUSIONISTS,
    UNIT_TYPE_MAGIC_FAMILIARS,
    UNIT_TYPE_NORTHERN_MONKS,
    UNIT_TYPE_RED_CAPE_MONKS,
    UNIT_TYPE_SAVAGE_MONKS,
    UNIT_TYPE_SHOCKTROOPS,
    UNIT_TYPE_UTEM_CROSSBOWMEN,
    UNIT_TYPE_UTEM_GUARDSMEN,
    UNIT_TYPE_UTEM_SWORDSMEN,
    UNIT_TYPE_AMOTEP_FREEZERS,
    UNIT_TYPE_AMOTEP_GUNNERS,
    UNIT_TYPE_CATAPULTS,
    UNIT_TYPE_ICE_GOLEMS,
    UNIT_TYPE_ICE_MAGES,
    UNIT_TYPE_FIRE_GOLEMS,
    UNIT_TYPE_FIRE_MAGES,
    UNIT_TYPE_HEROES_BLUE,
    UNIT_TYPE_HEROES_GREEN,
    UNIT_TYPE_HEROES_RED,
    UNIT_TYPE_HEROES_WHITE,
    UNIT_TYPE_SORCERERS,
    UNIT_TYPE_ALTEM_GUARDIANS,
    UNIT_TYPE_ALTEM_MAGES,
    UNIT_TYPE_DELPHANA_MASTERS,
    NUMBER_OF_UNIT_TYPES
}unit_type_t;

class Unit
{
public:
    Unit();

    // Public members
    QString mName = "";
    unsigned int mLevel = 0;
    bool mWounded = false;

    // Public methods
    unsigned int getScore(void);
};

#endif // UNIT_H
