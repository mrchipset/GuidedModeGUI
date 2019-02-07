#ifndef SCORECOMPUTATION_H
#define SCORECOMPUTATION_H

#include <QObject>
#include <QVector>
#include <armadillo>
#include "sChartWidget.h"
#include "cGloabalParam.h"
#include "Tools/sThread.h"

typedef struct S{
    QMutex mutex;
    int i;
    volatile int progress;
    double zS; //Zero Threshold
    //Dielectric Constant
    double n1;
    double n2;
    double n3;
    //Start and end frequency
    double f1;
    double f2;
    double fa;
    //Geometry Parmeters.
    double d;
    double p;
    QVector<double> * dVecF;
    QVector<double> * dVecBeta;

public:
    S(int i, double zS, double n1, double n2, double n3, double f1,
      double f2, double fa, double d, double p)
    {
        this->progress = 0;
        this->i = i;
        this->zS = zS;
        this->n1 = n1;
        this->n2 = n2;
        this->n3 = n3;
        this->f1 = f1;
        this->f2 = f2;
        this->fa = fa;
        this->d = d;
        this->p = p;
        this->dVecF = new QVector<double>();
        this->dVecBeta = new QVector<double>();
    }
}CoreParam;

class SCoreComputation : public QObject
{
    Q_OBJECT
public:
    explicit SCoreComputation(QObject * parent = nullptr);
    ~SCoreComputation();

public:
    void startProcess() { parentThread->sStart(); }
    void stopProcess() {
        parentThread->sStop();
        cleanWorker();
    }
    inline QString Log() const {return mLogger;}
signals:
    void drawBeta();
    void calcFinished();
    void logging();
    void updateProgress(int p);
private:
    void loadParam();
    void setGrating(double const& p);
    void setGrating();
    void addBoundary(double const& n);
    void addBoundaries();
    void buildLog();
    void calcProgress()
    {
        int sum = 0;
        for(auto var : pCoreParam)
            sum += var->progress;
        mProgress = sum / pCoreParam.size() + 1;
    }

    void cleanWorker()
    {
        for(auto var : pSThread)
        {
            var->sStop();
            var->terminate();
            delete var;
            var = nullptr;
        }
        pSThread.clear();

        for(auto var : pCoreParam)
        {
            var->mutex.unlock();
            delete var;
            var = nullptr;
        }
        pCoreParam.clear();
    }
    QString mLogger;
    SChartWidget * mChartWidget;

private:
    class CWorker : public QThread
    {
    public:
        explicit CWorker(QObject * parent = nullptr) : QThread(parent) {
            //mFunction = nullptr;
            mParam = nullptr;
            isRunning = false;
        }

    public:
        void sStop(){ isRunning = false; }//terminate();}
        void sStart(){ isRunning = true; start();}
        //void sSetUserFunc(const pSThreadUserFunc &func) { mFunction = func; }
        void sSetUserParam(void * pParm) { mParam = pParm; }

    protected:
        bool isRunning;
        //pSThreadUserFunc mFunction;
        void * mParam;
        int mTaskId;
        virtual void run() override;

    };

private:
    QVector<double> mGratingBeta;
    QVector<double> mBoundaryIndex;

    double mGratingPeroid;
    double mWaveGuideD;
    double mFreqParam;
    double mFreqStep;
    double mK;

    double mDielecParam1;
    double mDielecParam2;
    double mDielecParam3;

    double mZeroThreshold;
    int mWorkingThreads;
    volatile int mProgress;

    QVector<double> mQVecBeta;
    QVector<double> mQVecF;
    QList<CoreParam*> pCoreParam;
    QList<CWorker *> pSThread;
    QMutex mCoreMutex;
    QMutex mTaskMutex;

protected:
    SThread * parentThread;
    static int sCoreFunc(void * pParam, bool const & bRunning);



};

#endif // SCORECOMPUTATION_H
