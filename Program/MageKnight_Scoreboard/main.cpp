#include <QApplication>
#include <qobject.h>
#include "GameEngine.h"
#include "Window_MainBoard.h"
#include "Dialog_StartMenu.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setWindowIcon(QIcon(":/images/MageKnightLogo.jpg"));
    GameEngine gameEngine;
    Window_MainBoard mainBoard;
    Dialog_StartMenu startMenu;

    QObject::connect(&startMenu , SIGNAL(mageKnightPlayerAdded(QString, QString)                 ), &gameEngine, SLOT(on_mageKnightPlayerAdded(QString, QString)      ) );
    QObject::connect(&startMenu , SIGNAL(startNewGame()                                          ), &gameEngine, SLOT(on_newGameStarted()                             ) );
    QObject::connect(&startMenu , SIGNAL(resetGame()                                             ), &gameEngine, SLOT(on_gameReset()                                  ) );
    QObject::connect(&gameEngine, SIGNAL(newMageKnightPlayerListReady(const std::vector<Player>&)), &startMenu,  SLOT(on_playerListUpdated(const std::vector<Player>&)) );

    // If the connection is defined here, before the Start Menu is finished, the player data will appear immediately on the main board
    // Otherwise, the data will first appear one second (GameTimer triggered) after the main board is displayed.
    QObject::connect(&gameEngine, SIGNAL(newMageKnightPlayerData(const std::vector<Player>&, const GameTimer&)),      &mainBoard,  SLOT(on_newPlayerData(const std::vector<Player>&, const GameTimer&)));

    // Start the application by displaying the StartMenu window.
    if(startMenu.exec() == QDialog::Accepted)
    {
        mainBoard.showFullScreen();
        //QObject::connect(&gameEngine, SIGNAL(newMageKnightPlayerData(const std::vector<Player>&, const GameTimer&)),      &mainBoard,  SLOT(on_newPlayerData(const std::vector<Player>&, const GameTimer&)));

        // User action dialog related connections
        QObject::connect(&mainBoard,  SIGNAL(userActionDialogOpened()),                                                   &gameEngine, SLOT(on_userActionDialogOpened()));
        QObject::connect(&gameEngine, SIGNAL(newPlayerAndCityData(const std::vector<Player>&, const std::vector<City>&)), &mainBoard,  SLOT(on_newPlayerAndCityData(const std::vector<Player>&, const std::vector<City>&)));
        QObject::connect(&mainBoard,  SIGNAL(newUserAction(Action)),                                                      &gameEngine, SLOT(on_newUserAction(Action)));
        QObject::connect(&mainBoard,  SIGNAL(newTestUserAction(Action)),                                                  &gameEngine, SLOT(on_newTestUserAction(Action)));
        QObject::connect(&gameEngine, SIGNAL(newTempPlayerData(const Player&,const Player&)),                             &mainBoard,  SLOT(on_newTempPlayerData(const Player&, const Player&)));

        return a.exec(); // This runs the application by starting an event loop
    }
    else
    {
        return 0; // Terminates the application immediately
    }
}
