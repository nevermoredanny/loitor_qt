#include <iostream>
#include <QImage>
#include "camera_loitor.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_Cam_Loitor(new CAMERA_LOITOR),
    m_Show(NULL),
    m_simgforshow(NULL),
    m_BufferSize(1)
{
    m_grabSpace = new QSemaphore(m_BufferSize);
    m_showSpace = new QSemaphore(0);

    int width, height, depth;
    m_Cam_Loitor->m_Init( width, height, depth );
    m_simgforshow = new SteroImage(width, height, depth);

    m_Show = new ShowThread(this);
    connect( m_Show, SIGNAL(Signal_Show()), SLOT(m_Draw()) );
    m_Show->start();

    ui->setupUi(this);
}

int MainWindow::m_Draw()
{
    m_showSpace->release();
    std::cerr<<"S";
    QImage img_left( m_simgforshow->m_right_data, m_simgforshow->m_Width, m_simgforshow->m_Height, QImage::Format_Indexed8 );
    ui->left->setPixmap( QPixmap::fromImage(img_left, 0));
    ui->left->show();

    QImage img_right( m_simgforshow->m_right_data, m_simgforshow->m_Width, m_simgforshow->m_Height, QImage::Format_Indexed8 );
    ui->right->setPixmap( QPixmap::fromImage(img_right,0));
    ui->right->show();
    m_grabSpace->release();

    return 0;
}

int MainWindow::m_Grab()
{
    m_Cam_Loitor->m_Grab( *m_simgforshow );
}


MainWindow::~MainWindow()
{
    m_Show->wait();

    m_Cam_Loitor->m_Clear();

    delete m_simgforshow;
    m_simgforshow = NULL;

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
        m_ui->m_grabSpace->acquire();
        std::cerr<<"G";
        m_ui->m_Grab();
        emit Signal_Show();
        m_ui->m_showSpace->release();
    }
    std::cerr<<std::endl;
}
