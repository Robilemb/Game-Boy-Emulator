#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "GUI/Qt/INCLUDE/debugregisterswindow.h"
#include "GUI/Qt/INCLUDE/debugmemorywindow.h"
#include "CORE/INCLUDE/gameboy.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Gameboy*        getGameBoy();
    Ui::MainWindow* getUi();

private slots:
    void openDebugRegistersWindow();
    void openDebugMemoryWindow();

private:
    void closeEvent(QCloseEvent*);

private:
    Ui::MainWindow*         ui;

    DebugRegistersWindow*   mp_debugRegistersWindow;
    DebugMemoryWindow*      mp_debugMemoryWindow;

    Gameboy*                mp_gameboy;
};

#endif // MAINWINDOW_H
