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
    m_emulationIsRunning        = false;

    mp_debugRegistersWindow     = new DebugRegistersWindow(this);
    mp_debugMemoryWindow        = new DebugMemoryWindow(this);
}

MainWindow::~MainWindow()
{
    // Fin de l'émulation
    _stopEmulation();

    delete mp_debugMemoryWindow;
    delete mp_debugRegistersWindow;

    delete mp_gameboy;

    delete ui;
}

const Gameboy* MainWindow::getGameBoy() const
{
    return mp_gameboy;
}

Ui::MainWindow* MainWindow::getUi()
{
    return ui;
}

void MainWindow::openDebugRegistersWindow()
{
    mp_debugRegistersWindow->refresh();
    mp_debugRegistersWindow->show();
    ui->actionRegisters->setEnabled(false);
}

void MainWindow::openDebugMemoryWindow()
{
    mp_debugMemoryWindow->refresh(0);
    mp_debugMemoryWindow->show();
    ui->actionMemory->setEnabled(false);
}

void MainWindow::selectROMFileName()
{
    // Variable locale
    te_status w_status = E_UNDEFINED;

    // Selection de la ROM
    QString w_ROMFileName = QFileDialog::getOpenFileName(this,
                                                         tr("Selectionner la ROM"),
                                                         "./",
                                                         tr("ROM (*.gb)"));

    if (!w_ROMFileName.isNull())
    {
        // Chargement de la ROM
        w_status = mp_gameboy->loadROM(w_ROMFileName.toStdString());

        // Gestion des erreurs
        if (w_status == E_ERROR)
        {
            QMessageBox w_msgBox;
            w_msgBox.setText("ROM introuvable ou illisible.");
            w_msgBox.exec();
        }

        // Exécution de la ROM
        if (w_status == E_OK)
        {
            _startEmulation();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent*)
{
    qApp->quit();
}

void MainWindow::_startEmulation()
{
    // Fin du thread en cours d'exécution si rechargement d'une ROM
    _stopEmulation();

    // Thread d'exécution de la ROM
    m_gameboyThread = std::thread(&Gameboy::start, mp_gameboy);
    m_gameboyThread.detach();

    // Emulation en cours d'exécution
    m_emulationIsRunning = true;
}

void MainWindow::_stopEmulation()
{
    // Fin de l'émulation si une ROM était en cours de lecture
    if (m_emulationIsRunning)
    {
        mp_gameboy->stop();

        if (m_gameboyThread.joinable())
        {
            m_gameboyThread.join();
        }
    }
}
