#include "GameEngine.h"
#include "Window_MainBoard.h"
#include "Dialog_StartMenu.h"
#include <QApplication>
#include <qobject.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setWindowIcon(QIcon(":/images/MageKnightLogo.jpg"));
    GameEngine gameEngine;
    Window_MainBoard mainBoard;
    Dialog_StartMenu startMenu;

    QObject::connect(&startMenu,  SIGNAL(mageKnightPlayerAdded(QString, QString)),                  &gameEngine, SLOT(on_mageKnightPlayerAdded(QString, QString)) );
    QObject::connect(&gameEngine, SIGNAL(newMageKnightPlayerListReady(const std::vector<Player>&)), &startMenu,  SLOT(on_playerListUpdated(const std::vector<Player>&))  );

    // Start the application by displaying the StartMenu window.
    if(startMenu.exec() == QDialog::Accepted)
    {
        mainBoard.showFullScreen();
        return a.exec(); // This runs the application by starting an event loop
    }
    else
    {
        return 0; // Terminates the application immediately
    }
}
