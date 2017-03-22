#include "threadmanager.h"

QSemaphore ThreadManager::sp_grab2show;
QSemaphore ThreadManager::sp_grab2save;
QSemaphore ThreadManager::sp_show;
QSemaphore ThreadManager::sp_save;
QMutex ThreadManager::m_savemutex;
