#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void tmrDataGenerationTimer_Tick();

private:
    Ui::MainWindow *ui;
    void RegenerateXAxisData();

protected:
    virtual void keyPressEvent(QKeyEvent *KeyEventArgs);
    virtual void keyReleaseEvent(QKeyEvent *KeyEventArgs);
};

#endif // MAINWINDOW_H
