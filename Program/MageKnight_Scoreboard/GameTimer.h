#ifndef GAMETIMER_H
#define GAMETIMER_H

#include <QObject>
#include <QTimer>

class GameTimer : public QObject
{
    Q_OBJECT

public:
    // Constructor
    explicit GameTimer(QObject *parent = 0);

    // Public methods
    void startTimer();
    void stopTimer();
    bool isTimerActive();
    void setTimer(unsigned int h, unsigned int m, unsigned int s);

    // Public members
    unsigned int mTotalTimeInSeconds = 0;

signals:
    void gameTimerUpdate(unsigned int h, unsigned int m, unsigned int s);

private slots:
    void timeOutSlot();

private:    
    // Private members
    unsigned int mSeconds = 0;
    unsigned int mMinutes = 0;
    unsigned int mHours = 0;
    QTimer *mpTimer = new QTimer();
};

#endif // GAMETIMER_H
