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

    //const std::array<std::uint8_t, GAMEBOY_SCREEN_SIZE>& ai_image
    void refreshScreen(const std::string& ai_image);

private slots:
    void openDebugRegistersWindow();
    void openDebugMemoryWindow();
    void selectROMFileName();
    void setScreen(const std::string& ai_image);

signals:
    void setScreenSignal(const std::string& ai_image);

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
