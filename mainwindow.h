#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSemaphore>
#include <QThread>
#include <sys/time.h>

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

    QSemaphore *m_grabSpace;
    QSemaphore *m_showSpace;

    timeval m_left_stamp,m_right_stamp;
    uchar* m_left_buff;
    uchar* m_right_buff;
    int m_Width, m_Height;

    int m_Draw();

private:
    Ui::MainWindow *ui;

    CAMERA_LOITOR *m_Cam_Loitor;
    ShowThread *m_Show;

    const int m_BufferSize;

};

//// show
class ShowThread : public QThread
{

public:
    ShowThread(MainWindow* ui);

protected:
    void run();

private:
    MainWindow* m_ui;
};


#endif // MAINWINDOW_H
