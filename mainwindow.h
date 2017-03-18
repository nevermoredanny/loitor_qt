#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSemaphore>
#include <QThread>
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

    int m_Grab();

    QSemaphore *m_grabSpace;
    QSemaphore *m_showSpace;

private slots:
    int m_Draw();

private:
    Ui::MainWindow *ui;

    CAMERA_LOITOR *m_Cam_Loitor;
    ShowThread *m_Show;

    const int m_BufferSize;

    SteroImage *m_simgforshow;

};

// showthread
class ShowThread : public QThread
{
    Q_OBJECT

public:
    ShowThread(MainWindow* ui);

protected:
    void run();

signals:
    void Signal_Show();

private:
    MainWindow* m_ui;
};


#endif // MAINWINDOW_H
