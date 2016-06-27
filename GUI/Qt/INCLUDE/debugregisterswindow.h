#ifndef DEBUGREGISTERSWINDOW_H
#define DEBUGREGISTERSWINDOW_H

#include <QWidget>

namespace Ui
{
    class DebugRegistersWindow;
}

class DebugRegistersWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DebugRegistersWindow(QWidget *parent = 0);
    ~DebugRegistersWindow();

private:
    Ui::DebugRegistersWindow *ui;
};

#endif // DEBUGREGISTERSWINDOW_H
