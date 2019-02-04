#pragma once
#include <QThread>

//Define the user function pointer.
typedef int (*pSThreadUserFunc)(void * pParam,bool const & bRunning );

class SThread : public QThread
{
public:
    explicit SThread(QObject * parent = nullptr) : QThread(parent) {
        mFunction = nullptr;
        mParam = nullptr;
        isRunning = false;
    }

public:
    void sStop(){ isRunning = false; terminate();}
    void sStart(){ isRunning = true; start();}
    void sSetUserFunc(const pSThreadUserFunc &func) { mFunction = func; }
    void sSetUserParam(void * pParm) { mParam = pParm; }

protected:
    bool isRunning;
    pSThreadUserFunc mFunction;
    void * mParam;
    virtual void run() override;

};
