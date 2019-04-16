#include "GameTimer.h"

GameTimer::GameTimer(QObject *parent) : QObject(parent) {
    connect(&mpTimer, SIGNAL(timeout()), this, SLOT(on_TimeOut()));
}

void GameTimer::on_TimeOut() {
    // Slot connected with the QTimer instance signal (actual timer signal called every 1000 ms)
    ++mTicks;
    emit gameTimerUpdate();
}

void GameTimer::startTimer() {
    // Start the QTimer instance with a timeout signal call every 1000 ms
    mpTimer.start(1000);
}

void GameTimer::stopTimer() {
    mpTimer.stop();
}

bool GameTimer::isTimerActive() {
    return mpTimer.isActive();
}

void GameTimer::setTimer(unsigned int ticks) {
    mTicks = ticks;
    emit gameTimerUpdate();
}
