#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <thread>

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsScene>

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

    void refreshScreen(const gbScreenImage& ai_image);

private slots:
    void openDebugRegistersWindow();
    void openDebugMemoryWindow();
    void selectROMFileName();
    void setScreen(const gbScreenImage& ai_image);

signals:
    void setScreenSignal(const gbScreenImage& ai_image);

private:
    void closeEvent(QCloseEvent*);
    void _startEmulation();
    void _stopEmulation();

private:
    Ui::MainWindow*         ui;

    QGraphicsScene          m_screenScene;
    QGraphicsPixmapItem*    mp_screenImage;

    Gameboy*                mp_gameboy;
    std::thread             m_gameboyThread;
    bool                    m_emulationIsRunning;

    DebugRegistersWindow*   mp_debugRegistersWindow;
    DebugMemoryWindow*      mp_debugMemoryWindow;
};

#endif // MAINWINDOW_H
