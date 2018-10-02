#include "Window_StartMenu.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window_StartMenu w;
    w.show();

    return a.exec();
}
