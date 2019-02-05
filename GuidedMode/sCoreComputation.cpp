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
    mLogger.append("---------------Input Parameters---------------\n");
    mLogger.append("Waveguide Thinkness: ").append(QString::number(mWaveGuideD)).append(" m\n");
    mLogger.append("Grating Period: ").append(QString::number(mGratingPeroid)).append(" m\n");
    mLogger.append("Dielectirc Parameter 1: ").append(QString::number(mDielecParam1)).append("\n");
    mLogger.append("Dielectirc Parameter 2: ").append(QString::number(mDielecParam2)).append("\n");
    mLogger.append("Dielectirc Parameter 3: ").append(QString::number(mDielecParam3)).append("\n");
    mLogger.append("Calculation Freqency: ").append(QString::number(arma::datum::c_0/mFreqParam))
            .append(" Hz\taka Lambda: ").append(QString::number(mFreqParam)).append(" m\n");

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

        if(CGloabalParam::FREQ_PARAM_TYPE)
            mFreqParam = arma::datum::c_0 / CGloabalParam::FREQ_PARAM_VALUE.toDouble();
        else
            mFreqParam = CGloabalParam::FREQ_PARAM_VALUE.toDouble();

        mGratingPeroid = CGloabalParam::GEOMETRY_GRATING_P.toDouble();
        mWaveGuideD = CGloabalParam::GEOMETRY_WAVEGUIDE_D.toDouble();
        mK = 2 * arma::datum::pi / mFreqParam;
        CGloabalParam::GLOBAL_PARAM_MUTEX.unlock();
    }
}


int  SCoreComputation::sCoreFunc(void * pParam, bool const & bRunning)
{
    if(bRunning)
    {
        SCoreComputation * sParam = static_cast<SCoreComputation *>(pParam);
        sParam->mCoreMutex.lock();
        sParam->loadParam();
        sParam->buildLog();
        sParam->setGrating();
        sParam->mChartWidget->setGratingLines(sParam->mGratingBeta);
        sParam->addBoundaries();
        sParam->mChartWidget->setBoundaryIndex(sParam->mBoundaryIndex);

        emit(sParam->drawBeta(sParam->mGratingBeta));
        emit(sParam->calcFinished());
        sParam->mCoreMutex.unlock();
        return 1;
    }
    return 0;
}

