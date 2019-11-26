#include <QMetaType>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

#include "GUI/Qt/INCLUDE/mainwindow.h"
#include "ui_mainwindow.h"

Q_DECLARE_METATYPE(gbScreenImage)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_screenScene(this),
    mp_screenImage(nullptr),
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
    ui->screen->setScene(&m_screenScene);

    mp_screenImage = m_screenScene.addPixmap(QPixmap::fromImage(QImage()));

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
    mp_screenImage->setPixmap(QPixmap::fromImage(QImage(ai_image.data(), GAMEBOY_SCREEN_WIDTH, GAMEBOY_SCREEN_HEIGHT, QImage::Format_Grayscale8)));
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

void MainWindow::keyPressEvent(QKeyEvent* aip_keyPressEvent)
{
    if(aip_keyPressEvent->isAutoRepeat())
    {
        aip_keyPressEvent->ignore();
    }
    else
    {
        // Récupération de la liste des boutons pressés
        if (aip_keyPressEvent->key() == Qt::Key_Z)
            mp_gameboy->setPressButton(Gameboy::E_UP);
        if (aip_keyPressEvent->key() == Qt::Key_S)
            mp_gameboy->setPressButton(Gameboy::E_DOWN);
        if (aip_keyPressEvent->key() == Qt::Key_Q)
            mp_gameboy->setPressButton(Gameboy::E_LEFT);
        if (aip_keyPressEvent->key() == Qt::Key_D)
            mp_gameboy->setPressButton(Gameboy::E_RIGHT);
        if (aip_keyPressEvent->key() == Qt::Key_6)
            mp_gameboy->setPressButton(Gameboy::E_A);
        if (aip_keyPressEvent->key() == Qt::Key_5)
            mp_gameboy->setPressButton(Gameboy::E_B);
        if (aip_keyPressEvent->key() == Qt::Key_0)
            mp_gameboy->setPressButton(Gameboy::E_START);
        if (aip_keyPressEvent->key() == Qt::Key_Period)
            mp_gameboy->setPressButton(Gameboy::E_SELECT);
        if (aip_keyPressEvent->key() == Qt::Key_P)
            mp_gameboy->setDebug();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* aip_keyReleaseEvent)
{
    if(aip_keyReleaseEvent->isAutoRepeat())
    {
        aip_keyReleaseEvent->ignore();
    }
    else
    {
        // Récupération de la liste des boutons relachés
        if (aip_keyReleaseEvent->key() == Qt::Key_Z)
            mp_gameboy->setReleaseButton(Gameboy::E_UP);
        if (aip_keyReleaseEvent->key() == Qt::Key_S)
            mp_gameboy->setReleaseButton(Gameboy::E_DOWN);
        if (aip_keyReleaseEvent->key() == Qt::Key_Q)
            mp_gameboy->setReleaseButton(Gameboy::E_LEFT);
        if (aip_keyReleaseEvent->key() == Qt::Key_D)
            mp_gameboy->setReleaseButton(Gameboy::E_RIGHT);
        if (aip_keyReleaseEvent->key() == Qt::Key_6)
            mp_gameboy->setReleaseButton(Gameboy::E_A);
        if (aip_keyReleaseEvent->key() == Qt::Key_5)
            mp_gameboy->setReleaseButton(Gameboy::E_B);
        if (aip_keyReleaseEvent->key() == Qt::Key_0)
            mp_gameboy->setReleaseButton(Gameboy::E_START);
        if (aip_keyReleaseEvent->key() == Qt::Key_Period)
            mp_gameboy->setReleaseButton(Gameboy::E_SELECT);
    }
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
