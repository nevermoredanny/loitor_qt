#ifndef CAMERA_LOITOR_H
#define CAMERA_LOITOR_H

#include <sys/time.h>

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
            // err
            return *this;
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

class CAMERA_LOITOR
{

public:
    CAMERA_LOITOR();
    ~CAMERA_LOITOR();

    int m_Init( int &width, int &height, int &depth );
    int m_Grab( SteroImage& simg);
    int m_Clear();

    int m_Width;
    int m_Height;
};

#endif // CAMERA_LOITOR_H
