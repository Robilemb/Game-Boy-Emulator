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

    this->setFixedSize(260, 440);

    refresh();
}

DebugRegistersWindow::~DebugRegistersWindow()
{
    delete ui;
}

void DebugRegistersWindow::reject()
{
    static_cast<MainWindow*>(parent())->getUi()->actionRegisters->setEnabled(true);
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
    ui->labelRegisterA->setText("A = 0x" + QString::number(w_registerA, 16).toUpper());
    ui->labelRegisterB->setText("B = 0x" + QString::number(w_registerB, 16).toUpper());
    ui->labelRegisterC->setText("C = 0x" + QString::number(w_registerC, 16).toUpper());
    ui->labelRegisterD->setText("D = 0x" + QString::number(w_registerD, 16).toUpper());
    ui->labelRegisterE->setText("E = 0x" + QString::number(w_registerE, 16).toUpper());
    ui->labelRegisterF->setText("F = 0x" + QString::number(w_registerF, 16).toUpper());
    ui->labelRegisterH->setText("H = 0x" + QString::number(w_registerH, 16).toUpper());
    ui->labelRegisterL->setText("L = 0x" + QString::number(w_registerL, 16).toUpper());

    ui->labelRegisterAF->setText("AF = 0x" + QString::number(w_registerAF, 16).toUpper());
    ui->labelRegisterBC->setText("BC = 0x" + QString::number(w_registerBC, 16).toUpper());
    ui->labelRegisterDE->setText("DE = 0x" + QString::number(w_registerDE, 16).toUpper());
    ui->labelRegisterHL->setText("HL = 0x" + QString::number(w_registerHL, 16).toUpper());
    ui->labelRegisterPC->setText("PC = 0x" + QString::number(w_registerPC, 16).toUpper());
    ui->labelRegisterSP->setText("SP = 0x" + QString::number(w_registerSP, 16).toUpper());

    ui->labelFlagZ->setText("Z = " + QString::number(w_flagZ));
    ui->labelFlagN->setText("N = " + QString::number(w_flagN));
    ui->labelFlagH->setText("H = " + QString::number(w_flagH));
    ui->labelFlagC->setText("C = " + QString::number(w_flagC));
}
