#include "GUI/Qt/INCLUDE/debugmemorywindow.h"
#include "ui_debugmemorywindow.h"

DebugMemoryWindow::DebugMemoryWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DebugMemoryWindow)
{
    ui->setupUi(this);
}

DebugMemoryWindow::~DebugMemoryWindow()
{
    delete ui;
}
