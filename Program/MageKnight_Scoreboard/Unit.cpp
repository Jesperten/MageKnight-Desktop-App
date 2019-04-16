#include "Unit.h"

Unit::Unit()
{

}


unsigned int Unit::getScore(void)
{
    unsigned int score = 0;

    if (mWounded)
        score = mLevel/2;
    else
        score = mLevel;

    return score;
}
