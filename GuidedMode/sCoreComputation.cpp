#include "sCoreComputation.h"

SCoreComputation::SCoreComputation(QObject * parent)
    : QObject (parent)
{
    //Initilize the Qwt Plot Singleton Instance.
    mChartWidget = SChartWidget::GetInstance();
    parentThread = new SThread();
    parentThread->sSetUserFunc(sCoreFunc);
    parentThread->sSetUserParam(this);
    mProgress = 0;
}

SCoreComputation::~SCoreComputation()
{
    for(auto var : pSThread)
        var->sStop();
    mChartWidget = nullptr;
}

void SCoreComputation::setGrating(double const& p)
{
    mGratingBeta.clear();
    for(int i = 2; i <= 6; i+=2)
        mGratingBeta.push_back(i*arma::datum::pi/p);
}

void SCoreComputation::setGrating()
{
  setGrating(mGratingPeroid);
}

void SCoreComputation::addBoundary(double const& n)
{
    mBoundaryIndex.push_back(arma::datum::c_0/n);
}

void SCoreComputation::addBoundaries()
{
    mBoundaryIndex.clear();
    addBoundary(mDielecParam1);
    addBoundary(mDielecParam2);
    addBoundary(mDielecParam3);
}

void SCoreComputation::buildLog()
{
    mLogger.clear();
    mLogger.append("Calculation Started.\n");
    mLogger.append("Working Threads: ").append(QString::number(mWorkingThreads)).append("\n");
    mLogger.append("---------------Input Parameters---------------\n");
    mLogger.append("Waveguide Thinkness: ").append(QString::number(mWaveGuideD)).append(" m\n");
    mLogger.append("Grating Period: ").append(QString::number(mGratingPeroid)).append(" m\n");
    mLogger.append("Dielectirc Parameter 1: ").append(QString::number(mDielecParam1)).append("\n");
    mLogger.append("Dielectirc Parameter 2: ").append(QString::number(mDielecParam2)).append("\n");
    mLogger.append("Dielectirc Parameter 3: ").append(QString::number(mDielecParam3)).append("\n");
    mLogger.append("Calculation Freqency: ").append(QString::number(mFreqParam))
            .append(" Hz\taka Lambda: ").append(QString::number(arma::datum::c_0/mFreqParam)).append(" m\n");

    emit(logging());
}

void SCoreComputation::loadParam()
{
    if(CGloabalParam::GLOBAL_PARAM_MUTEX.tryLock(50))
    {
        //Load Dielectric Parameters
        if(CGloabalParam::DIELEC_PARAM_TYPE)
        {
            //Convert Dielectric constant to Refraction Index
        }
        else{
            mDielecParam1 = CGloabalParam::DIELEC_PARAM_1.toDouble();
            mDielecParam2 = CGloabalParam::DIELEC_PARAM_2.toDouble();
            mDielecParam3 = CGloabalParam::DIELEC_PARAM_3.toDouble();
        }

        if(!CGloabalParam::FREQ_PARAM_TYPE)
            mFreqParam = arma::datum::c_0 / CGloabalParam::FREQ_PARAM_VALUE.toDouble();
        else
            mFreqParam = CGloabalParam::FREQ_PARAM_VALUE.toDouble();

        mGratingPeroid = CGloabalParam::GEOMETRY_GRATING_P.toDouble();
        mWaveGuideD = CGloabalParam::GEOMETRY_WAVEGUIDE_D.toDouble();
        mK = 2 * arma::datum::pi / mFreqParam;
        mWorkingThreads = 1<<CGloabalParam::COCURRENT;
        qDebug()<<"mWorkingThreads: "<<mWorkingThreads;
        CGloabalParam::GLOBAL_PARAM_MUTEX.unlock();
    }
}


int SCoreComputation::sCoreFunc(void * pParam, bool const & bRunning)
{
    if(bRunning)
    {
        SCoreComputation * sParam = static_cast<SCoreComputation *>(pParam);
        sParam->mCoreMutex.lock();
        sParam->mProgress = 0;
        sParam->loadParam();
        sParam->buildLog();
        sParam->setGrating();
        sParam->mChartWidget->setGratingLines(sParam->mGratingBeta);
        sParam->addBoundaries();
        sParam->mChartWidget->setBoundaryIndex(sParam->mBoundaryIndex);
        emit(sParam->drawBeta(sParam->mGratingBeta));
        //Arrange Task; Will be an function in future;
        double step = sParam->mFreqParam / sParam->mWorkingThreads;
        for(int i=0;i<sParam->mWorkingThreads;i++)
            sParam->pCoreParam.append(new CoreParam(i,sParam->mDielecParam1,sParam->mDielecParam2,
                                                sParam->mDielecParam3,i*step,(i+1)*step,0.0001e12,
                                                sParam->mWaveGuideD,sParam->mGratingPeroid));
        qDebug()<<"pCoreParam Size: "<<sParam->pCoreParam.size();
        for(int i=0;i<sParam->mWorkingThreads;i++)
            sParam->pSThread.append(new CWorker());

        for(auto var: sParam->pSThread)
        {
            var->sSetUserParam(sParam);
            var->sStart();
            QThread::msleep(10);
        }
        int i = 0;
        while(bRunning && sParam->mProgress < 100)
        {
            sParam->calcProgress();
            emit(sParam->updateProgress(sParam->mProgress));
            //qDebug()<<"mProgress:"<<sParam->mProgress;
            i++;
            QThread::msleep(500);
        }
        qDebug()<<"Used Time:"<<i/2<<"s";
        emit(sParam->calcFinished());
        sParam->cleanWorker();
        sParam->mCoreMutex.unlock();
        return 1;
    }
    return 0;
}

void SCoreComputation::CWorker::run()
{
    if(isRunning)
    {
        SCoreComputation * pParam = static_cast<SCoreComputation *>(mParam);
        mTaskId = -1;
        //find a no-assigned task;
        while(isRunning && mTaskId == -1)
        {
            if(pParam->mTaskMutex.tryLock(50))
            {

                for(auto var : pParam->pCoreParam)
                {
                    qDebug()<<var->i;
                    if(var->mutex.tryLock())
                    {
                        mTaskId = var->i;
                        break;
                    }
                }
                pParam->mTaskMutex.unlock();
            }
            qDebug()<<"Now TaskId:"<<mTaskId;
            QThread::msleep(100);
        }
        qDebug()<<"Thread: "<<mTaskId<<"Working";
        const double n1 = pParam->pCoreParam[mTaskId]->n1;
        const double n2 = pParam->pCoreParam[mTaskId]->n2;
        const double n3 = pParam->pCoreParam[mTaskId]->n3;
        const double d = pParam->pCoreParam[mTaskId]->d;
        const double p = pParam->pCoreParam[mTaskId]->p;
        const double f1 = pParam->pCoreParam[mTaskId]->f1;
        const double f2 = pParam->pCoreParam[mTaskId]->f2;
        const double fa = pParam->pCoreParam[mTaskId]->fa;

        arma::vec theta;
        arma::vec kapa, delta, gamma;
        arma::vec func;
        double lam, k;
        theta = arma::linspace(0,arma::datum::pi/2,10000);
        for(double f = f1; f < f2 && isRunning; f+=fa)
        {
            lam = arma::datum::c_0 / f;
            k = 2 * arma::datum::pi / lam;
            kapa = n2*k*arma::sin(theta);
            gamma = arma::sqrt((n2*n2-n1*n1)*k*k-kapa%kapa);
            delta = arma::sqrt((n2*n2-n3*n3)*k*k-kapa%kapa);
            func = arma::tan(arma::sin(theta)*d*n2*k) - (n2*n2*kapa*d) % (n3*n3*gamma+n1*n1*delta)*d
                    / (n1*n1*n3*n3*(kapa%kapa)*d*d - n2*n2*n2*n2*(gamma%delta)*d*d);
            func = arma::abs(func);
            arma::uvec index = arma::find(func<1e-4);
            //qDebug()<<"Freq: "<<f<<"Hz\tMin Value: "<<func.min()<<"\tThreshold Size: "<<index.size();
            //To-Do
            //Implement a filter to find useful theta;
            //Convbert index into theta use formula beta = n*k*cos(theta);

            pParam->pCoreParam[mTaskId]->progress = static_cast<int>((f-f1) * 100 / (f2-f1));
        }
        qDebug()<<"Thread: "<<mTaskId<<"Finished";
    }
}

