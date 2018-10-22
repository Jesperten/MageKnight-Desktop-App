#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "City.h"
#include "Player.h"
#include <QString>
#include <QObject>

class GameEngine : public QObject
{
    Q_OBJECT

public:
    GameEngine();
   ~GameEngine();

signals:
    // The mMageKnight_Player and _Cities must be send as read-only (const) pointers to improve speed and avoid data-access from other scopes.
    void newMageKnightPlayerListReady(const std::vector<Player>& mageKnightPlayerList); // For startWindow only
    void newMageKnightPlayerData(const std::vector<Player>& mageKnightPlayerList); // For the MainWindow only

public slots:
    void on_mageKnightPlayerAdded(QString playerName, QString playerCharacter);

private:
    // Private Members
    std::vector<Player> mMageKnight_Player; // List of player objects (specified as pointer for faster cross-references)
    std::vector<City>   mMageKnight_Cities; // List of discovered cities on the map (specified as pointer for faster cross-references)
    // TODO: The character name strings should be defined in GameEngine (not in dropdown list in StartWindow) as it could be needed elsewhere in later versions.
};

#endif // GAMEENGINE_H
