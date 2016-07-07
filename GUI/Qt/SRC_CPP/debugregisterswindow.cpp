#include "GUI/Qt/INCLUDE/debugregisterswindow.h"
#include "ui_debugregisterswindow.h"
#include "GUI/Qt/INCLUDE/mainwindow.h"
#include "ui_mainwindow.h"
#include "CORE/INCLUDE/cpu.h"
#include "CORE/INCLUDE/gameboy.h"

DebugRegistersWindow::DebugRegistersWindow(QWidget *ai_parent) :
    QDialog(ai_parent),
    ui(new Ui::DebugRegistersWindow)
{
    ui->setupUi(this);

    this->setFixedSize(230, 440);

    refresh();
}

DebugRegistersWindow::~DebugRegistersWindow()
{
    delete ui;
}

void DebugRegistersWindow::reject()
{
    static_cast<MainWindow*>(parent())->ui->actionRegisters->setEnabled(true);
    QDialog::reject();
}

void DebugRegistersWindow::refresh()
{
    // Registres 8 bits
    std::uint8_t w_registerA = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getRegisterA();
    std::uint8_t w_registerB = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getRegisterB();
    std::uint8_t w_registerC = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getRegisterC();
    std::uint8_t w_registerD = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getRegisterD();
    std::uint8_t w_registerE = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getRegisterE();
    std::uint8_t w_registerF = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getRegisterF();
    std::uint8_t w_registerH = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getRegisterH();
    std::uint8_t w_registerL = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getRegisterL();

    // Registres 16 bits
    std::uint16_t w_registerAF = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getRegisterAF();
    std::uint16_t w_registerBC = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getRegisterBC();
    std::uint16_t w_registerDE = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getRegisterDE();
    std::uint16_t w_registerHL = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getRegisterHL();
    std::uint16_t w_registerPC = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getRegisterPC();
    std::uint16_t w_registerSP = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getRegisterSP();

    // Flags
    std::uint8_t w_flagZ = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getFlagZ();
    std::uint8_t w_flagN = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getFlagN();
    std::uint8_t w_flagH = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getFlagH();
    std::uint8_t w_flagC = static_cast<MainWindow*>(parent())->getGameBoy()->getCpu()->getFlagC();

    // Mise à jour des labels
    ui->labelRegisterA->setText("A = " + QString::number(w_registerA));
    ui->labelRegisterB->setText("B = " + QString::number(w_registerB));
    ui->labelRegisterC->setText("C = " + QString::number(w_registerC));
    ui->labelRegisterD->setText("D = " + QString::number(w_registerD));
    ui->labelRegisterE->setText("E = " + QString::number(w_registerE));
    ui->labelRegisterF->setText("F = " + QString::number(w_registerF));
    ui->labelRegisterH->setText("H = " + QString::number(w_registerH));
    ui->labelRegisterL->setText("L = " + QString::number(w_registerL));

    ui->labelRegisterAF->setText("AF = " + QString::number(w_registerAF));
    ui->labelRegisterBC->setText("BC = " + QString::number(w_registerBC));
    ui->labelRegisterDE->setText("DE = " + QString::number(w_registerDE));
    ui->labelRegisterHL->setText("HL = " + QString::number(w_registerHL));
    ui->labelRegisterPC->setText("PC = " + QString::number(w_registerPC));
    ui->labelRegisterSP->setText("SP = " + QString::number(w_registerSP));

    ui->labelFlagZ->setText("Z = " + QString::number(w_flagZ));
    ui->labelFlagN->setText("N = " + QString::number(w_flagN));
    ui->labelFlagH->setText("H = " + QString::number(w_flagH));
    ui->labelFlagC->setText("C = " + QString::number(w_flagC));
}
