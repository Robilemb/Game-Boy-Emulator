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
