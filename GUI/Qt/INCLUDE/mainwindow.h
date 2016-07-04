#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "GUI/Qt/INCLUDE/debugregisterswindow.h"
#include "GUI/Qt/INCLUDE/debugmemorywindow.h"
#include "CORE/INCLUDE/cpu.h"

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

    Cpu getCpu();

private slots:
    void openDebugRegistersWindow();
    void openDebugMemoryWindow();

    void on_pushButton_clicked();

private:
    void closeEvent(QCloseEvent*);

private:
    Ui::MainWindow* ui;

    DebugRegistersWindow*   m_debugRegistersWindow;
    DebugMemoryWindow*      m_debugMemoryWindow;

    Cpu                     m_cpu;
};

#endif // MAINWINDOW_H
