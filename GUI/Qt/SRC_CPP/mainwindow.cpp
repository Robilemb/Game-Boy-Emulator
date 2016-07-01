#include "GUI/Qt/INCLUDE/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Game Boy Emulator");
    this->setWindowIcon(QIcon("../IMG/nintendo-game-boy-icon.png"));

    debugRegistersWindow    = new DebugRegistersWindow();
    debugMemoryWindow       = new DebugMemoryWindow();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete debugRegistersWindow;
}

void MainWindow::openDebugRegistersWindow()
{
    debugRegistersWindow->show();
}

void MainWindow::openDebugMemoryWindow()
{
    debugMemoryWindow->show();
}

void MainWindow::closeEvent(QCloseEvent*)
{
    qApp->quit();
}
