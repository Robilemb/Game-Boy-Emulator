#include "GUI/Qt/INCLUDE/debugmemorywindow.h"
#include "ui_debugmemorywindow.h"
#include "GUI/Qt/INCLUDE/mainwindow.h"
#include "ui_mainwindow.h"

DebugMemoryWindow::DebugMemoryWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugMemoryWindow)
{
    ui->setupUi(this);

    this->setFixedSize(470, 340);

    ui->upButton->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
    ui->downButton->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));

    refresh(0);
}

DebugMemoryWindow::~DebugMemoryWindow()
{
    delete ui;
}

void DebugMemoryWindow::reject()
{
    static_cast<MainWindow*>(parent())->getUi()->actionMemory->setEnabled(true);
    QDialog::reject();
}

void DebugMemoryWindow::refreshDebugMemoryWindow()
{
    // Mise à jour de la table
    refresh(getIdxInFirstRow());
}

void DebugMemoryWindow::refreshUpDebugMemoryWindow()
{
    // Variables locales
    std::int32_t w_idxAddress = getIdxInFirstRow();

    // Gestion des bords
    if (w_idxAddress - 1 < 0)
    {
        w_idxAddress = (static_cast<std::int32_t>(MPU_MEMORY_SIZE) - 1);
    }
    else
    {
        w_idxAddress--;
    }

    // Mise à jour de la table
    refresh(w_idxAddress);
}

void DebugMemoryWindow::refreshDownDebugMemoryWindow()
{
    // Variables locales
    std::int32_t w_idxAddress = getIdxInFirstRow();

    // Gestion des bords
    if (w_idxAddress + 1 > (static_cast<std::int32_t>(MPU_MEMORY_SIZE) - 1))
    {
        w_idxAddress = 0;
    }
    else
    {
        w_idxAddress++;
    }

    // Mise à jour de la table
    refresh(w_idxAddress);
}

void DebugMemoryWindow::displayAtSelectedMemoryAddress()
{
    // Variables locales
    std::uint16_t w_address = 0u;

    // Récupération de l'adresse à afficher
    w_address = this->ui->spinBox->value();

    // Mise à jour de la table
    refresh(w_address);
}

void DebugMemoryWindow::displayAtDefinedMemoryAddress()
{
    // Variables locales
    std::uint8_t  w_index   = 0u;
    std::uint16_t w_address = 0u;

    // Récupération de l'index selectionné
    w_index = this->ui->comboBox->currentIndex();

    // Récupération de l'adresse à afficher
    switch(w_index)
    {
        case 0u:
            w_address = 0x0;
            break;

        case 1u:
            w_address = 0x4000;
            break;

        case 2u:
            w_address = 0x8000;
            break;

        case 3u:
            w_address = 0xA000;
            break;

        case 4u:
            w_address = 0xC000;
            break;

        case 5u:
            w_address = 0xD000;
            break;

        case 6u:
            w_address = 0xE000;
            break;

        case 7u:
            w_address = 0xFE00;
            break;

        case 8u:
            w_address = 0xFF00;
            break;

        case 9u:
            w_address = 0xFF04;
            break;

        case 10u:
            w_address = 0xFF05;
            break;

        case 11u:
            w_address = 0xFF06;
            break;

        case 12u:
            w_address = 0xFF07;
            break;

        case 13u:
            w_address = 0xFF0F;
            break;

        case 14u:
            w_address = 0xFF40;
            break;

        case 15u:
            w_address = 0xFF42;
            break;

        case 16u:
            w_address = 0xFF43;
            break;

        case 17u:
            w_address = 0xFF44;
            break;

        case 18u:
            w_address = 0xFF47;
            break;

        case 19u:
            w_address = 0xFF4A;
            break;

        case 20u:
            w_address = 0xFF4B;
            break;

        case 21u:
            w_address = 0xFF80;
            break;

        case 22u:
            w_address = 0xFFFF;
        break;
    }

    // Mise à jour de la table
    refresh(w_address);
}

void DebugMemoryWindow::refresh(std::uint16_t ai_idx)
{
    // Variables loacales
    std::uint8_t        w_nbItems       = ui->tableMemoryWidget->rowCount();
    std::uint8_t        w_memValue      = 0u;
    std::uint16_t       w_idxMemValue   = 0u;
    std::uint32_t       w_idxAddress    = 0u;
    QTableWidgetItem*   wp_curItem      = nullptr;
    QStringList         w_addressList;

    // Mise à jour des items
    for (std::uint16_t w_i = 0u; w_i < w_nbItems; ++w_i)
    {
        // Adresse à afficher
        w_idxAddress = ai_idx + w_i;
        if (w_idxAddress > (static_cast<std::int32_t>(MPU_MEMORY_SIZE) - 1))
        {
            w_idxAddress -= static_cast<std::int32_t>(MPU_MEMORY_SIZE);
        }

        w_addressList << "0x" + QString::number(w_idxAddress, 16).toUpper();

        // Valeur en mémoire à afficher
        w_idxMemValue = ai_idx + w_i;
        w_memValue = static_cast<MainWindow*>(parent())->getGameBoy()->getMpu()->getMemVal(w_idxMemValue);

        // Mise à jour de l'item
        wp_curItem = new QTableWidgetItem();
        wp_curItem->setText("0x" + QString::number(w_memValue, 16).toUpper());
        wp_curItem->setTextAlignment(Qt::AlignCenter);
        ui->tableMemoryWidget->setItem(w_i, 0, wp_curItem);
    }

    // Mise à jour de l'affichage des adresses
    ui->tableMemoryWidget->setVerticalHeaderLabels(w_addressList);
}

std::uint16_t DebugMemoryWindow::getIdxInFirstRow() const
{
    // Variables locales
    QString             w_strIdxAddress;
    std::uint16_t       w_idxAddress;
    QTableWidgetItem*   w_curItem;

    // Récupération de l'index mémoire affichée à la première ligne
    w_curItem       = ui->tableMemoryWidget->verticalHeaderItem(0);
    w_strIdxAddress = w_curItem->text();
    w_strIdxAddress.remove("0x");
    w_idxAddress    = w_strIdxAddress.toUInt(Q_NULLPTR, 16);

    return w_idxAddress;
}
