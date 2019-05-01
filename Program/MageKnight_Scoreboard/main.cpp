#include <QApplication>
#include <qobject.h>
#include "GameEngine.h"
#include "Window_MainBoard.h"
#include "Dialog_StartMenu.h"

#include "Dialog_UserAction.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setWindowIcon(QIcon(":/images/MageKnightLogo.jpg"));
    Window_MainBoard mainBoard;
    Dialog_StartMenu startMenu;

    const GameEngine* gameEngine = GameEngine::instance(); // Const pointer to the Singleton instance of the GameEngine class (global object)

    // ******************************** Connections between GameEngine and Start Menu ************************************************************************************
    QObject::connect(&startMenu , &Dialog_StartMenu::playerAdded           ,  gameEngine,  &GameEngine::on_mageKnightPlayerAdded   );
    QObject::connect(&startMenu , &Dialog_StartMenu::startNewGame          ,  gameEngine,  &GameEngine::on_newGameStarted          );
    QObject::connect(&startMenu , &Dialog_StartMenu::resetGame             ,  gameEngine,  &GameEngine::on_gameReset               );
    QObject::connect( gameEngine, &GameEngine::newMageKnightPlayerListReady, &startMenu ,  &Dialog_StartMenu::on_playerListUpdated );
    // *******************************************************************************************************************************************************************

    // If the connection is defined here, before the Start Menu is finished, the player data will appear immediately on the main board
    // Otherwise, the data will first appear one second (GameTimer triggered) after the main board is displayed.
    QObject::connect(gameEngine, &GameEngine::newMageKnightData, &mainBoard,  &Window_MainBoard::on_newMageKnightData);

    // Start the application by displaying the StartMenu window.
    if(startMenu.exec() == QDialog::Accepted) {
        mainBoard.showFullScreen();
        QObject::connect(&mainBoard,  &Window_MainBoard::userActionDialogOpened, gameEngine, &GameEngine::on_userActionDialogOpened);
        QObject::connect(&mainBoard,  &Window_MainBoard::playPauseToggle,        gameEngine, &GameEngine::on_pausePlayToggle);
        QObject::connect(gameEngine,  &GameEngine::gamePlayPauseState,           &mainBoard, &Window_MainBoard::on_gamePlayPauseState);
        return a.exec(); // This runs the application by starting an event loop
    }
    else {
        return 0; // Terminates the application immediately
    }
}
