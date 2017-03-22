#include <iostream>
#include <QImage>
#include <QDir>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <boost/thread.hpp>
#include "camera_loitor.h"
#include "threadmanager.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_Cam_Loitor(new CAMERA_LOITOR)
{
    ui->setupUi(this);

    connect( m_Cam_Loitor, SIGNAL(grabbed()), SLOT(m_Show()) );
    connect( ui->label_left, SIGNAL(clicked_right()), SLOT(m_Save()) );
    connect( ui->label_right, SIGNAL(clicked_right()), SLOT(m_Save()) );
    connect( ui->label_left, SIGNAL(clicked_left()), SLOT(m_SaveLeft()) );
    connect( ui->label_right, SIGNAL(clicked_left()), SLOT(m_SaveRight()) );

    int width, height, depth;
    m_Cam_Loitor->m_Init( width, height, depth );
    m_Cam_Loitor->m_Open();

}

void MainWindow::m_Show()
{
    ThreadManager::sp_show.acquire();

    QImage img_left( CAMERA_LOITOR::m_Simg_Show.m_left_data,
                     CAMERA_LOITOR::m_Simg_Show.m_Width,
                     CAMERA_LOITOR::m_Simg_Show.m_Height,
                     QImage::Format_Indexed8 );
    ui->label_left->setPixmap( QPixmap::fromImage(img_left, 0));
    ui->label_left->show();

    QImage img_right( CAMERA_LOITOR::m_Simg_Show.m_right_data,
                      CAMERA_LOITOR::m_Simg_Show.m_Width,
                      CAMERA_LOITOR::m_Simg_Show.m_Height,
                      QImage::Format_Indexed8 );
    ui->label_left->setPixmap( QPixmap::fromImage(img_left, 0));
    ui->label_left->show();
    ui->label_right->setPixmap( QPixmap::fromImage(img_right,0));
    ui->label_right->show();
    ThreadManager::sp_grab2show.release();

    return;
}

void MainWindow::m_SaveLeft()
{

    QMutexLocker locker(&ThreadManager::m_savemutex);
    ThreadManager::sp_grab2save.release();
    ThreadManager::sp_save.acquire();
    std::cerr << "F";

    QString Path = QCoreApplication::applicationDirPath() + QString("/Pic/left");
    QDir dir(Path);
    if(!dir.exists())
    {
        bool ok = dir.mkpath(Path);
    }

    QString str_left = Path +
                       QString("/") +
                       QString::number(CAMERA_LOITOR::m_Simg_Procee.m_left_stamp.tv_sec) +
                       QString::number(CAMERA_LOITOR::m_Simg_Procee.m_left_stamp.tv_usec) +
                       QString(".png");

    cv::Mat mat_left( CAMERA_LOITOR::m_Simg_Procee.m_Height,
                      CAMERA_LOITOR::m_Simg_Procee.m_Width,
                      CV_8UC1,
                      CAMERA_LOITOR::m_Simg_Procee.m_left_data );

    cv::imwrite( str_left.toStdString(), mat_left );

    return;
}

void MainWindow::m_SaveRight()
{

    QMutexLocker locker(&ThreadManager::m_savemutex);
    ThreadManager::sp_grab2save.release();
    ThreadManager::sp_save.acquire();
    std::cerr << "F";

    QString Path = QCoreApplication::applicationDirPath() + QString("/Pic/right");
    QDir dir(Path);
    if(!dir.exists())
    {
        bool ok = dir.mkpath(Path);
    }

    QString str_right = Path +
                        QString("/") +
                        QString::number(CAMERA_LOITOR::m_Simg_Procee.m_right_stamp.tv_sec) +
                        QString::number(CAMERA_LOITOR::m_Simg_Procee.m_right_stamp.tv_usec) +
                        QString(".png");

    cv::Mat mat_right( CAMERA_LOITOR::m_Simg_Procee.m_Height,
                       CAMERA_LOITOR::m_Simg_Procee.m_Width,
                       CV_8UC1,
                       CAMERA_LOITOR::m_Simg_Procee.m_right_data );

    cv::imwrite( str_right.toStdString(), mat_right );

    return;
}

void MainWindow::m_Save()
{

    QMutexLocker locker(&ThreadManager::m_savemutex);
    ThreadManager::sp_grab2save.release();
    ThreadManager::sp_save.acquire();
    std::cerr << "F";

    QString Path = QCoreApplication::applicationDirPath() + QString("/Pic/stero");
    QDir dir(Path);
    if(!dir.exists())
    {
        bool ok = dir.mkpath(Path);
    }

    QString str_left = Path +
                       QString("/left_") +
                       QString::number(CAMERA_LOITOR::m_Simg_Procee.m_left_stamp.tv_sec) +
                       QString::number(CAMERA_LOITOR::m_Simg_Procee.m_left_stamp.tv_usec) +
                       QString(".png");

    QString str_right = Path +
                        QString("/right_") +
                        QString::number(CAMERA_LOITOR::m_Simg_Procee.m_right_stamp.tv_sec) +
                        QString::number(CAMERA_LOITOR::m_Simg_Procee.m_right_stamp.tv_usec) +
                        QString(".png");

    cv::Mat mat_left( CAMERA_LOITOR::m_Simg_Procee.m_Height,
                      CAMERA_LOITOR::m_Simg_Procee.m_Width,
                      CV_8UC1,
                      CAMERA_LOITOR::m_Simg_Procee.m_left_data );
    cv::Mat mat_right( CAMERA_LOITOR::m_Simg_Procee.m_Height,
                       CAMERA_LOITOR::m_Simg_Procee.m_Width,
                       CV_8UC1,
                       CAMERA_LOITOR::m_Simg_Procee.m_right_data );

    cv::imwrite( str_left.toStdString(), mat_left );
    cv::imwrite( str_right.toStdString(), mat_right );

    return;
}

MainWindow::~MainWindow()
{
    m_Cam_Loitor->m_Close();
    m_Cam_Loitor->wait();
    m_Cam_Loitor->m_Clear();

    delete m_Cam_Loitor;
    m_Cam_Loitor = NULL;

    delete ui;
}

