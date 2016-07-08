#include "GUI/Qt/INCLUDE/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Game Boy Emulator");
    this->setWindowIcon(QIcon("../IMG/Icon.png"));

    mp_gameboy                  = new Gameboy();

    mp_debugRegistersWindow     = new DebugRegistersWindow(this);
    mp_debugMemoryWindow        = new DebugMemoryWindow(this);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete mp_debugRegistersWindow;
    delete mp_debugMemoryWindow;

    delete mp_gameboy;
}

Gameboy* MainWindow::getGameBoy()
{
    return mp_gameboy;
}

Ui::MainWindow* MainWindow::getUi()
{
    return ui;
}

void MainWindow::openDebugRegistersWindow()
{
    mp_debugRegistersWindow->show();
    ui->actionRegisters->setEnabled(false);
}

void MainWindow::openDebugMemoryWindow()
{
    mp_debugMemoryWindow->show();
    ui->actionMemory->setEnabled(false);
}

void MainWindow::closeEvent(QCloseEvent*)
{
    qApp->quit();
}
