#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <thread>

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

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

    const Gameboy*      getGameBoy() const;
    Ui::MainWindow*     getUi();

private slots:
    void openDebugRegistersWindow();
    void openDebugMemoryWindow();
    void selectROMFileName();

private:
    void closeEvent(QCloseEvent*);
    void _startEmulation();
    void _stopEmulation();

private:
    Ui::MainWindow*         ui;

    Gameboy*                mp_gameboy;
    std::thread             m_gameboyThread;
    bool                    m_emulationIsRunning;

    DebugRegistersWindow*   mp_debugRegistersWindow;
    DebugMemoryWindow*      mp_debugMemoryWindow;
};

#endif // MAINWINDOW_H
