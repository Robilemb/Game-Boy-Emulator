#include "GUI/Qt/INCLUDE/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Game Boy Emulator");
    this->setWindowIcon(QIcon("../IMG/nintendo-game-boy-icon.png"));

    m_debugRegistersWindow  = new DebugRegistersWindow(this);
    m_debugMemoryWindow     = new DebugMemoryWindow(this);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete m_debugRegistersWindow;
    delete m_debugMemoryWindow;
}

Cpu MainWindow::getCpu()
{
    return m_cpu;
}

void MainWindow::openDebugRegistersWindow()
{
    m_debugRegistersWindow->show();
    ui->actionRegisters->setEnabled(false);
}

void MainWindow::openDebugMemoryWindow()
{
    m_debugMemoryWindow->show();
    ui->actionMemory->setEnabled(false);
}

void MainWindow::closeEvent(QCloseEvent*)
{
    qApp->quit();
}
