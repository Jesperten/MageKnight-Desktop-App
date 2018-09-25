//#include <QTimer.h>
#include "GameTimer.h"

GameTimer::GameTimer(QObject *parent) : QObject(parent)
{
    connect(mpTimer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
}

void GameTimer::timeOutSlot()
{
    if (++mSeconds > 59)
    {
        mSeconds = 0;

        if (++mMinutes > 59)
        {
            mMinutes = 0;
            ++mHours;
        }
    }

    ++mTotalTimeInSeconds;

    emit gameTimerUpdate(mHours, mMinutes, mSeconds);
}

void GameTimer::startTimer()
{
    mpTimer->start(1000);
}

void GameTimer::stopTimer()
{
    mpTimer->stop();
}

bool GameTimer::isTimerActive()
{
    return mpTimer->isActive();
}

void GameTimer::setTimer(unsigned int h, unsigned int m, unsigned int s)
{
    mHours = h;
    mMinutes = m;
    mSeconds = s;

    emit gameTimerUpdate(mHours, mMinutes, mSeconds);
}
