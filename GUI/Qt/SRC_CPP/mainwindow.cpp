#include <QMetaType>

#include "GUI/Qt/INCLUDE/mainwindow.h"
#include "ui_mainwindow.h"

Q_DECLARE_METATYPE(gbScreenImage)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_screenImage(this),
    mp_gameboy(new Gameboy(std::bind(&MainWindow::refreshScreen, this, std::placeholders::_1))),
    m_emulationIsRunning(false),
    mp_debugRegistersWindow(new DebugRegistersWindow(this)),
    mp_debugMemoryWindow(new DebugMemoryWindow(this))
{
    // Fenetre principale
    ui->setupUi(this);

    this->setWindowTitle("Game Boy Emulator");
    this->setWindowIcon(QIcon("../IMG/Icon.png"));
    this->setFixedSize(GAMEBOY_SCREEN_WIDTH, (GAMEBOY_SCREEN_HEIGHT + 20u));

    // Initialisation de l'écran
    ui->screen->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->screen->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->screen->setScene(&m_screenImage);

    // Connection entre le signal de MAJ de l'écran (commandé par mp_gameboy) et le slot de MAJ de l'écran
    qRegisterMetaType<gbScreenImage>();
    QObject::connect(this, SIGNAL(setScreenSignal(const gbScreenImage&)),
                     this, SLOT(setScreen(const gbScreenImage&)));
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

void MainWindow::setScreen(const gbScreenImage& ai_image)
{
    m_screenImage.addPixmap(QPixmap::fromImage(QImage(ai_image.data(), GAMEBOY_SCREEN_WIDTH, GAMEBOY_SCREEN_HEIGHT, QImage::Format_Grayscale8)));
}

void MainWindow::refreshScreen(const gbScreenImage& ai_image)
{
    // Emission du signal commandant la MAJ de l'écran
    emit setScreenSignal(ai_image);
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
        // Fin du thread en cours d'exécution si rechargement d'une ROM
        _stopEmulation();

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
