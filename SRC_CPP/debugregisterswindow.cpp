#include "INCLUDE/debugregisterswindow.h"
#include "ui_debugregisterswindow.h"

DebugRegistersWindow::DebugRegistersWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DebugRegistersWindow)
{
    ui->setupUi(this);
}

DebugRegistersWindow::~DebugRegistersWindow()
{
    delete ui;
}
