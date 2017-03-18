#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSemaphore>
#include <QThread>
#include <QMutex>
#include <sys/time.h>

class SteroImage;
class CAMERA_LOITOR;
class ShowThread;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int m_Grab2Show();

    int m_Grab2Save();

    QSemaphore *m_grab2showSpace;
    QSemaphore *m_showSpace;
    QSemaphore *m_saveSpace;
//    QMutex memmutex;

private slots:
    void m_Show();
    void m_Save();

signals:
    void Signal_Show();

private:
    Ui::MainWindow *ui;

    CAMERA_LOITOR *m_Cam_Loitor;
    ShowThread *m_ShowThread;

    const int m_BufferSize;

    SteroImage *m_simgforshow;
    SteroImage *m_simgforsave;
};

// showthread
class ShowThread : public QThread
{
    Q_OBJECT

public:
    ShowThread(MainWindow* ui);

    void stop();

protected:
    void run();

private:
    MainWindow* m_ui;
    bool flag;
};


#endif // MAINWINDOW_H
