#include "Action.h"

Action::Action() {
    mMonsters.resize(0);
    mUnits.resize(0);
}

void Action::reset() {
    mPlayerID = 0;
    mMainActionID = 0;
    mOptionalID = 0;
    mSuccessFail = false;
    mWounds = 0;
    mTime = 0;
    mCityID = 0;

    mAddAAC = 0;
    mAddArtifacts = 0;
    mAddCrystals = 0;
    mAddFame = 0;
    mAddRepStep = 0;
    mAddSpells = 0;

    mMonsters.clear();
    mMonsters.resize(0);
    mUnits.clear();
    mUnits.resize(0);
}

void Action::addMonster(unsigned int MonsterCount) {
    unsigned int N = mMonsters.size();
    mMonsters.resize(N+MonsterCount);
}

void Action::removeMonster(unsigned int MonsterCount) {
    int N = int(mMonsters.size());
    int newN = N - int(MonsterCount);

    if (newN > 0) mMonsters.resize(unsigned(newN));
    else          mMonsters.resize(0);
}

void Action::addUnit(unsigned int UnitCount) {
    unsigned int N = mUnits.size();
    mUnits.resize(N+UnitCount);
}

void Action::removeUnit(unsigned int UnitCount) {
    int N = int(mUnits.size());
    int newN = N - int(UnitCount);

    if (newN > 0) mUnits.resize(unsigned(newN));
    else          mUnits.resize(0);
}
