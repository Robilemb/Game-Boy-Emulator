#include "INCLUDE/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication w_emulator( argc, argv );

    MainWindow w_mainWindow;
    w_mainWindow.setWindowTitle("Game Boy Emulator");
    w_mainWindow.show();

    return w_emulator.exec();
}
