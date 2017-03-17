#ifndef CAMERA_LOITOR_H
#define CAMERA_LOITOR_H

#include "mainwindow.h"
#include <QThread>

class CAMERA_LOITOR : public QThread
{

public:
    CAMERA_LOITOR(MainWindow* ui);
    ~CAMERA_LOITOR();

    int m_Init();
    int m_Clear();

    int m_Width;
    int m_Height;

protected:
    void run();

private:
    MainWindow *m_ui;

};

#endif // CAMERA_LOITOR_H
