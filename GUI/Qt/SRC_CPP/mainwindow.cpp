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
    mp_debugRegistersWindow->refresh();
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

void MainWindow::selectROMFileName()
{
    // Variable locale
    Status w_status = E_UNDEFINED;

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
            mp_gameboy->run();
        }
    }
}
