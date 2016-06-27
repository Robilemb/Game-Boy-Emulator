#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "GUI/Qt/INCLUDE/debugregisterswindow.h"
#include "GUI/Qt/INCLUDE/debugmemorywindow.h"

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

private slots:
    void openDebugRegistersWindow();
    void openDebugMemoryWindow();

private:
    Ui::MainWindow* ui;

    DebugRegistersWindow*   debugRegistersWindow;
    DebugMemoryWindow*      debugMemoryWindow;
};

#endif // MAINWINDOW_H
