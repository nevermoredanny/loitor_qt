#include <QDebug>
#include <QCoreApplication>

#include <string>
#include <iostream>
#include <opencv2/core.hpp>
#include <boost/thread.hpp>

#include "loitorusbcam.h"
#include "loitorimu.h"

#include "camera_loitor.h"

CAMERA_LOITOR::CAMERA_LOITOR() :
    m_Width(IMG_WIDTH_WVGA),
    m_Height(IMG_HEIGHT_WVGA)
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

int CAMERA_LOITOR::m_Grab(SteroImage &simg)
{
    visensor_imudata paired_imu = visensor_get_stereoImg((char *)simg.m_left_data,
                                                         (char *)simg.m_right_data,
                                                                 simg.m_left_stamp,
                                                                 simg.m_right_stamp);
    return 0;
}

int CAMERA_LOITOR::m_Clear()
{

    /* close cameras */
    visensor_Close_Cameras();
    /* close IMU */
    visensor_Close_IMU();

    return 0;
}
