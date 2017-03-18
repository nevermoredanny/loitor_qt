#include <iostream>
#include <QImage>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "camera_loitor.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_Cam_Loitor(new CAMERA_LOITOR),
    m_ShowThread(NULL),
    m_simgforshow(NULL),
    m_simgforsave(NULL),
    m_BufferSize(1)
{
    ui->setupUi(this);

    connect( this, SIGNAL(Signal_Show()), SLOT(m_Show()) );
    connect( ui->label_left, SIGNAL(clicked()), SLOT(m_Save()) );
    connect( ui->label_right, SIGNAL(clicked()), SLOT(m_Save()) );

    m_grab2showSpace = new QSemaphore(m_BufferSize);
    m_showSpace = new QSemaphore(0);

    int width, height, depth;
    m_Cam_Loitor->m_Init( width, height, depth );
    m_simgforshow = new SteroImage(width, height, depth);
    m_simgforsave = new SteroImage(width, height, depth);

    m_ShowThread = new ShowThread(this);
    m_ShowThread->start();


}

void MainWindow::m_Show()
{
//    QMutexLocker locker(&memmutex);
    m_showSpace->acquire();
//    std::cerr<<"S";
    QImage img_left( m_simgforshow->m_left_data, m_simgforshow->m_Width, m_simgforshow->m_Height, QImage::Format_Indexed8 );
    ui->label_left->setPixmap( QPixmap::fromImage(img_left, 0));
    ui->label_left->show();

    QImage img_right( m_simgforshow->m_right_data, m_simgforshow->m_Width, m_simgforshow->m_Height, QImage::Format_Indexed8 );
    ui->label_right->setPixmap( QPixmap::fromImage(img_right,0));
    ui->label_right->show();
    m_grab2showSpace->release();

    return;
}

void MainWindow::m_Save()
{
//    QMutexLocker locker(&memmutex);
    std::cerr << "F";

    m_Cam_Loitor->m_Grab( *m_simgforsave );

    QString str_left = QCoreApplication::applicationDirPath() +
                       QString("/Pic/left_") +
                       QString::number(m_simgforsave->m_left_stamp.tv_sec) +
                       QString::number(m_simgforsave->m_left_stamp.tv_usec) +
                       QString(".png");

    QString str_right = QCoreApplication::applicationDirPath() +
                       QString("/Pic/right_") +
                       QString::number(m_simgforsave->m_right_stamp.tv_sec) +
                       QString::number(m_simgforsave->m_right_stamp.tv_usec) +
                       QString(".png");

#if 1
    cv::Mat mat_left( m_simgforsave->m_Height, m_simgforsave->m_Width, CV_8UC1, m_simgforsave->m_left_data );
    cv::Mat mat_right( m_simgforsave->m_Height, m_simgforsave->m_Width, CV_8UC1, m_simgforsave->m_right_data );

    cv::imwrite( str_left.toStdString(), mat_left );
    cv::imwrite( str_right.toStdString(), mat_right );
#else
    QImage img_left( m_simgforshow->m_right_data, m_simgforshow->m_Width, m_simgforshow->m_Height, QImage::Format_Indexed8 );
    QImage img_right( m_simgforshow->m_right_data, m_simgforshow->m_Width, m_simgforshow->m_Height, QImage::Format_Indexed8 );
    img_left.save(str_left, "png");
    img_right.save(str_right, "png");
#endif

    return;
}

int MainWindow::m_Grab2Show()
{
    m_grab2showSpace->acquire();
//    std::cerr<<"G";
    m_Cam_Loitor->m_Grab( *m_simgforshow );
    m_showSpace->release();
    emit Signal_Show();
    return 0;
}

MainWindow::~MainWindow()
{
    m_ShowThread->stop();
    m_ShowThread->wait();
    delete m_ShowThread;
    m_ShowThread = NULL;

    delete m_grab2showSpace;
    m_grab2showSpace = NULL;

    delete m_simgforsave;
    m_simgforsave = NULL;

    delete m_simgforshow;
    m_simgforshow = NULL;

    delete m_showSpace;
    m_showSpace = NULL;

    m_Cam_Loitor->m_Clear();
    delete m_Cam_Loitor;
    m_Cam_Loitor = NULL;

    delete ui;
}

ShowThread::ShowThread( MainWindow* ui ) :
    flag(true)
{
    m_ui = ui;
}

void ShowThread::stop()
{
    flag = false;
}

void ShowThread::run()
{
    while(flag)
    {
        m_ui->m_Grab2Show();
    }
    std::cerr<<std::endl;
}
