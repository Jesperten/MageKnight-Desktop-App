#ifndef GAMETIMER_H
#define GAMETIMER_H

#include <QObject>
#include <QTimer>

class GameTimer : public QObject
{
    Q_OBJECT
    
public:
    // Constructor
    explicit GameTimer(QObject *parent = nullptr);
    
    // Public methods
    void startTimer();
    void stopTimer();
    bool isTimerActive();
    void setTimer(unsigned int ticks);
    
    // Public members
    unsigned long mTicks = 0;

signals:
    void gameTimerUpdate();
    
private slots:
    void on_TimeOut();

private:
    // Private members
    QTimer mpTimer;
};

#endif // GAMETIMER_H
