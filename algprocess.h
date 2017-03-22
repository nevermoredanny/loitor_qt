#ifndef ALGPROCESS_H
#define ALGPROCESS_H

class SteroImage;
class CAMERA_LOITOR;
class string;

namespace Alg {

class OnlineAlgProcess
{
public:
    OnlineAlgProcess();
    ~OnlineAlgProcess();

    int m_Init( const CAMERA_LOITOR& cam );

    int m_Process( const SteroImage &input, SteroImage &output );

    int m_Clear();

private:

};

class MonoCalib
{
public:
    MonoCalib();
    ~MonoCalib();

    int m_Init( const CAMERA_LOITOR& cam );

    int m_Process( const SteroImage &input, SteroImage &output );

    int m_Clear();
};

}





#endif // ALGPROCESS_H
