#ifndef DEBUGMEMORYWINDOW_H
#define DEBUGMEMORYWINDOW_H

#include <QDialog>
#include <QStyle>

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

    void refresh(std::uint16_t ai_idx);

private slots:
    void refreshDebugMemoryWindow();
    void refreshUpDebugMemoryWindow();
    void refreshDownDebugMemoryWindow();
    void displayAtSelectedMemoryAddress();
    void displayAtDefinedMemoryAddress();

private:
    void reject();
    std::uint16_t getIdxInFirstRow();

private:
    Ui::DebugMemoryWindow *ui;
};

#endif // DEBUGMEMORYWINDOW_H
