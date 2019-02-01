#include "sThread.h"

void SThread::run()
{
    isRunning = true;
    mFunction(mParam, isRunning);
    isRunning = false;
}
