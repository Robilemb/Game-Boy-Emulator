#include "GUI/Qt/INCLUDE/debugmemorywindow.h"
#include "ui_debugmemorywindow.h"
#include "GUI/Qt/INCLUDE/mainwindow.h"
#include "ui_mainwindow.h"

DebugMemoryWindow::DebugMemoryWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugMemoryWindow)
{
    ui->setupUi(this);
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
