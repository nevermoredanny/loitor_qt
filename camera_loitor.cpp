#include <QDebug>
#include <QCoreApplication>

#include <iostream>

#include "loitorusbcam.h"
#include "loitorimu.h"

#include "camera_loitor.h"

CAMERA_LOITOR::CAMERA_LOITOR(MainWindow* ui) :
    m_ui(ui),
    m_Width(IMG_WIDTH_WVGA),
    m_Height(IMG_HEIGHT_WVGA)
{

}

CAMERA_LOITOR::~CAMERA_LOITOR()
{

}

int CAMERA_LOITOR::m_Init()
{
    /************************ Start Cameras ************************/
    QString str = QCoreApplication::applicationDirPath() + QString("/Loitor_VISensor_Setups.txt");
    QByteArray config = str.toLatin1();
    visensor_load_settings( config.data() );

    // 手动设置相机参数
    //visensor_set_current_mode(5);
    //visensor_set_auto_EG(0);
    //visensor_set_exposure(50);
    //visensor_set_gain(200);
    //visensor_set_cam_selection_mode(2);
    //visensor_set_resolution(false);
    //visensor_set_fps_mode(true);
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

//    CamThread cam;
//    ShowThread show;
//    cam.start();
//    show.start();
//    cam.wait();
//    show.wait();

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

void CAMERA_LOITOR::run()
{
    while(true)
    {
        m_ui->m_grabSpace->acquire();
        std::cerr<<"P";
        visensor_imudata paired_imu = visensor_get_stereoImg((char *)m_ui->m_left_buff,
                                                             (char *)m_ui->m_right_buff,
                                                             m_ui->m_left_stamp,
                                                             m_ui->m_right_stamp);
        m_ui->m_showSpace->release();
    }
    std::cerr<<std::endl;
}
