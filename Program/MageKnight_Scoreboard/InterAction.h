#ifndef INTERACTION_H
#define INTERACTION_H

#include <QStringList>
#include <vector>
#include "Unit.h"

class InterAction
{
public:
    InterAction();
    // Public members
    unsigned int mPlayerID;
    unsigned int mMainInterActionID;
    unsigned int mHealedWounds;
    unsigned int mTime;

    int mBoughtSpells;
    int mBoughtAACs;
    int mBoughtArtifacts;

    std::vector<Unit> mRecruitedUnits;
    std::vector<Unit> mHealedUnits;

    // Additional values (besides from the main action)
    unsigned int mAddFame;
    int mAddRepStep;

    // Public methods
    void reset();
    void updateResults();
    void addUnit(unsigned int UnitCount);
    void removeUnit(unsigned int UnitCount);
    int getFame();
    int getRep();
    int getSpellCards();
    int getAAC();
    int getArtifacts();

private:
    // Private members
    int mFame = 0;
    int mRepStep = 0;
    int mSpellCard = 0;
    int mAAC = 0;
    int mArtifact = 0;
};

#endif // INTERACTION_H
