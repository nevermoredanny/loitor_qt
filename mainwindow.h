#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSemaphore>
#include <QThread>
#include <QMutex>
#include <sys/time.h>

class CAMERA_LOITOR;

enum cam_stype
{
    left,
    right,
    stero
};

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
    void m_Show();
    void m_Save();
    void m_SaveLeft();
    void m_SaveRight();

private:
    Ui::MainWindow *ui;

    CAMERA_LOITOR *m_Cam_Loitor;
};

#endif
