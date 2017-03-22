#include <QDebug>
#include <QCoreApplication>

#include <string>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "loitorusbcam.h"
#include "loitorimu.h"

#include "threadmanager.h"
#include "camera_loitor.h"

SteroImage CAMERA_LOITOR::m_Simg_Show(IMG_WIDTH_WVGA, IMG_HEIGHT_WVGA, 1);
SteroImage CAMERA_LOITOR::m_Simg_Procee(IMG_WIDTH_WVGA, IMG_HEIGHT_WVGA, 1);

CAMERA_LOITOR::CAMERA_LOITOR() :
    m_Width(IMG_WIDTH_WVGA),
    m_Height(IMG_HEIGHT_WVGA),
    m_Runnable(true)
{

}

CAMERA_LOITOR::~CAMERA_LOITOR()
{

}

int CAMERA_LOITOR::m_Init( int &width, int &height, int &depth )
{
    width = m_Width;
    height = m_Height;
    depth = 1;

    /************************ Start Cameras ************************/
    QString str = QCoreApplication::applicationDirPath() + QString("/Loitor_VISensor_Setups.txt");
    QByteArray config = str.toLatin1();
    visensor_load_settings( config.data() );

    // 手动设置相机参数
//    visensor_set_current_mode(5);
//    visensor_set_auto_EG(0);
//    visensor_set_exposure(50);
//    visensor_set_gain(200);
//    visensor_set_cam_selection_mode(2);
//    visensor_set_resolution(false);
//    visensor_set_fps_mode(true);
    // 保存相机参数到原配置文件
    //visensor_save_current_settings();

    int r = visensor_Start_Cameras();
    if(r<0)
    {
        qDebug("Opening cameras failed...\r\n");
        return r;
    }
    /************************** Start IMU **************************/
    int fd=visensor_Start_IMU();
    if(fd<0)
    {
        qDebug("visensor_open_port error...\r\n");
        return 0;
    }
    qDebug("visensor_open_port success...\r\n");
    /************************ ************ ************************/

    return 0;
}

int CAMERA_LOITOR::m_Open()
{
    m_Runnable = true;
    start();
    ThreadManager::sp_grab2show.release();
    return 0;
}

int CAMERA_LOITOR::m_Close()
{
    m_Runnable = false;
    return 0;
}

const SteroImage& CAMERA_LOITOR::m_GetSimg2Show()
{
    return m_Simg_Show;
}

const SteroImage& CAMERA_LOITOR::m_GetSimg2Process()
{
    return m_Simg_Procee;
}

int CAMERA_LOITOR::m_Clear()
{

    /* close cameras */
    visensor_Close_Cameras();
    /* close IMU */
    visensor_Close_IMU();

    return 0;
}

int CAMERA_LOITOR::m_Grab(SteroImage &simg)
{
    visensor_imudata paired_imu = visensor_get_stereoImg((char *)simg.m_left_data,
                                                         (char *)simg.m_right_data,
                                                                 simg.m_left_stamp,
                                                                 simg.m_right_stamp);
    return 0;
}

void CAMERA_LOITOR::run()
{

    while( m_Runnable )
    {
        ThreadManager::sp_grab2show.acquire();
        visensor_imudata paired_imu = visensor_get_stereoImg((char *)m_Simg_Show.m_left_data,
                                                             (char *)m_Simg_Show.m_right_data,
                                                                     m_Simg_Show.m_left_stamp,
                                                                     m_Simg_Show.m_right_stamp);
        emit grabbed();
        ThreadManager::sp_show.release();

        if(ThreadManager::sp_grab2save.tryAcquire())
        {
//            ThreadManager::sp_grab2save.acquire();
            m_Simg_Procee = m_Simg_Show;
            ThreadManager::sp_save.release();
        }
    }

}

int CAMERA_LOITOR::m_SaveOneTime(const char *left, const char *right)
{
    cv::Mat mat_left( CAMERA_LOITOR::m_Simg_Procee.m_Height,
                      CAMERA_LOITOR::m_Simg_Procee.m_Width,
                      CV_8UC1,
                      CAMERA_LOITOR::m_Simg_Procee.m_left_data );
    cv::Mat mat_right( CAMERA_LOITOR::m_Simg_Procee.m_Height,
                       CAMERA_LOITOR::m_Simg_Procee.m_Width,
                       CV_8UC1,
                       CAMERA_LOITOR::m_Simg_Procee.m_right_data );

    cv::imwrite( left, mat_left );
    cv::imwrite( right, mat_right );
    return 0;
}
