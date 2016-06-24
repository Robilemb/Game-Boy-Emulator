#ifndef DEBUGMEMORYWINDOW_H
#define DEBUGMEMORYWINDOW_H

#include <QWidget>

namespace Ui
{
    class DebugMemoryWindow;
}

class DebugMemoryWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DebugMemoryWindow(QWidget *parent = 0);
    ~DebugMemoryWindow();

private:
    Ui::DebugMemoryWindow *ui;
};

#endif // DEBUGMEMORYWINDOW_H
