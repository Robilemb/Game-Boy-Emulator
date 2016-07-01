#include "GUI/Qt/INCLUDE/debugregisterswindow.h"
#include "ui_debugregisterswindow.h"

DebugRegistersWindow::DebugRegistersWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DebugRegistersWindow)
{
    ui->setupUi(this);

    this->setFixedSize(230, 440);
}

DebugRegistersWindow::~DebugRegistersWindow()
{
    delete ui;
}
