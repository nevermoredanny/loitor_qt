#ifndef CAMERA_LOITOR_H
#define CAMERA_LOITOR_H

#include <sys/time.h>
#include <QThread>

class SteroImage
{
public:
    SteroImage( int width, int height, int depth ) :
        m_Width(width),
        m_Height(height),
        m_Depth(depth)
    {
        m_left_data = new unsigned char[m_Width*m_Height];
        m_right_data = new unsigned char[m_Width*m_Height];
        m_left_stamp.tv_sec = 0;
        m_left_stamp.tv_usec = 0;
        m_right_stamp.tv_sec = 0;
        m_right_stamp.tv_usec = 0;
    }

    ~SteroImage()
    {
        delete [] m_left_data;
        m_left_data = NULL;

        delete [] m_right_data;
        m_right_data = NULL;
    }

    SteroImage& operator = (const SteroImage& simg)
    {
        if(this == &simg)
            return *this;

        if(this->m_Width != simg.m_Width ||
           this->m_Height != simg.m_Height ||
           this->m_Depth != simg.m_Depth )
        {
            delete [] m_left_data;
            m_left_data = NULL;

            delete [] m_right_data;
            m_right_data = NULL;

            m_left_data = new unsigned char[simg.m_Width*simg.m_Height];
            m_right_data = new unsigned char[simg.m_Width*simg.m_Height];

        }

        this->m_Width = simg.m_Width;
        this->m_Height = simg.m_Height;
        this->m_Depth = simg.m_Depth;
        this->m_left_stamp = simg.m_left_stamp;
        this->m_right_stamp = simg.m_right_stamp;

        memcpy( this->m_left_data, simg.m_left_data, this->m_Width * this->m_Height );
        memcpy( this->m_right_data, simg.m_right_data, this->m_Width * this->m_Height );

        return *this;
    }

    int m_Width;
    int m_Height;
    int m_Depth;
    uchar* m_left_data;
    uchar* m_right_data;
    timeval m_left_stamp;
    timeval m_right_stamp;
};

class CAMERA_LOITOR : public QThread
{
    Q_OBJECT

public:
    CAMERA_LOITOR();
    ~CAMERA_LOITOR();

    int m_Init( int &width, int &height, int &depth );

    int m_Open();

    const SteroImage& m_GetSimg2Show();

    const SteroImage& m_GetSimg2Process();

    int m_Grab( SteroImage& simg);

    int m_Close();

    int m_Clear();

protected:
    void run();

    int m_Width;
    int m_Height;

    bool m_Runnable;

signals:
    void grabbed();

public:

    static SteroImage m_Simg_Show;

    static SteroImage m_Simg_Procee;

    static int m_SaveOneTime( const char *left, const char *right );

};

#endif // CAMERA_LOITOR_H
