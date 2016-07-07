#ifndef DEBUGMEMORYWINDOW_H
#define DEBUGMEMORYWINDOW_H

#include <QDialog>

namespace Ui
{
    class DebugMemoryWindow;
}

class DebugMemoryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DebugMemoryWindow(QWidget *parent = 0);
    ~DebugMemoryWindow();

private:
    void reject();

private:
    Ui::DebugMemoryWindow *ui;
};

#endif // DEBUGMEMORYWINDOW_H
