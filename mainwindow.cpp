#include <iostream>
#include <QImage>
#include "camera_loitor.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_Cam_Loitor(NULL),
    m_Show(NULL),
    m_left_buff(NULL),
    m_right_buff(NULL),
    m_BufferSize(1)
{
    m_grabSpace = new QSemaphore(m_BufferSize);
    m_showSpace = new QSemaphore(0);

    m_Cam_Loitor = new CAMERA_LOITOR(this);
    m_Width = m_Cam_Loitor->m_Width;
    m_Height = m_Cam_Loitor->m_Height;
    m_left_buff = new unsigned char[m_Width*m_Height];
    m_right_buff = new unsigned char[m_Width*m_Height];


    m_Cam_Loitor->m_Init();
    m_Cam_Loitor->start();

    m_Show = new ShowThread(this);
    m_Show->start();

    ui->setupUi(this);
}

int MainWindow::m_Draw()
{
    QImage img_left( m_left_buff, m_Width, m_Height, QImage::Format_RGB888 );
    ui->left->setPixmap( QPixmap::fromImage(
                             img_left.scaled( ui->left->width(), ui->left->height()).rgbSwapped())
                         );

    QImage img_right( m_right_buff, m_Width, m_Height, QImage::Format_RGB888 );
    ui->right->setPixmap( QPixmap::fromImage(
                             img_right.scaled( ui->right->width(), ui->right->height()).rgbSwapped())
                         );
    return 0;
}

MainWindow::~MainWindow()
{
//    m_Cam_Loitor->wait();

    m_Show->wait();

    m_Cam_Loitor->m_Clear();

    delete [] m_right_buff;
    m_right_buff = NULL;

    delete [] m_left_buff;
    m_left_buff = NULL;

    delete m_grabSpace;
    m_grabSpace = NULL;

    delete m_showSpace;
    m_showSpace = NULL;

    delete m_Show;
    m_Show = NULL;

    delete m_Cam_Loitor;
    m_Cam_Loitor = NULL;

    delete ui;
}

ShowThread::ShowThread( MainWindow* ui )
{
    m_ui = ui;
}

void ShowThread::run()
{
    while(true)
    {
        m_ui->m_showSpace->acquire();
        std::cerr<<"C";
//        m_ui->m_Draw();
        m_ui->m_grabSpace->release();
    }
    std::cerr<<std::endl;
}
