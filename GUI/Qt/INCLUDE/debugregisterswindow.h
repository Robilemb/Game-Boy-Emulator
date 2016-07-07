#ifndef DEBUGREGISTERSWINDOW_H
#define DEBUGREGISTERSWINDOW_H

#include <QDialog>

namespace Ui
{
    class DebugRegistersWindow;
}

class DebugRegistersWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DebugRegistersWindow(QWidget *parent = 0);
    ~DebugRegistersWindow();

    void refresh();

private:
    void reject();

private:
    Ui::DebugRegistersWindow *ui;
};

#endif // DEBUGREGISTERSWINDOW_H
