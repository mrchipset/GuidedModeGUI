#include "sCoreComputation.h"

SCoreComputation::SCoreComputation(QObject * parent)
    : QObject (parent)
{
    //Initilize the Qwt Plot Singleton Instance.
    mChartWidget = SChartWidget::GetInstance();
    parentThread = new SThread();
    parentThread->sSetUserFunc(sCoreFunc);
    parentThread->sSetUserParam(this);

}

SCoreComputation::~SCoreComputation()
{
    mChartWidget = nullptr;
}

void SCoreComputation::getGrating(double const p)
{
    mGratingBeta.clear();
    for(int i = 2; i <= 6; i+=2)
        mGratingBeta.push_back(i*arma::datum::pi/p);
}

void SCoreComputation::getGrating()
{
    CGloabalParam::GLOBAL_PARAM_MUTEX.lock();
    getGrating(CGloabalParam::GEOMETRY_GRATING_P.toDouble());
    CGloabalParam::GLOBAL_PARAM_MUTEX.unlock();
}


int  SCoreComputation::sCoreFunc(void * pParam, bool const & bRunning)
{
    if(bRunning)
    {
        qDebug()<<"Into the sub Thread.";
        SCoreComputation * sParam = static_cast<SCoreComputation *>(pParam);
        sParam->getGrating();
        for(auto var : sParam->mGratingBeta)
            qDebug()<<var;
        emit(sParam->drawBeta(sParam->mGratingBeta));

        return 1;
    }
    return 0;
}

