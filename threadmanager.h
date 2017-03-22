#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QSemaphore>
#include <QMutex>

class ThreadManager
{
public:
    static QSemaphore sp_grab2show;
    static QSemaphore sp_grab2save;
    static QSemaphore sp_show;
    static QSemaphore sp_save;

    static QMutex m_savemutex;
};



#endif // THREADMANAGER_H
