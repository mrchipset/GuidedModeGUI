#ifndef SCORECOMPUTATION_H
#define SCORECOMPUTATION_H

#include <QObject>
#include <QVector>
#include <armadillo>
#include "sChartWidget.h"
#include "cGloabalParam.h"
#include "Tools/sThread.h"

class SCoreComputation : public QObject
{
    Q_OBJECT
public:
    explicit SCoreComputation(QObject * parent = nullptr);
    ~SCoreComputation();

public:
    void startProcess() { parentThread->sStart(); }
    void stopProcess() { parentThread->sStop(); }
    inline QString Log() const {return mLogger;}
signals:
    void drawBeta(QVector<double>);
    void calcFinished();
    void logging();
private:
    void loadParam();
    void setGrating(double const& p);
    void setGrating();
    void addBoundary(double const& n);
    void addBoundaries();
    void buildLog();
    QString mLogger;
    SChartWidget * mChartWidget;

private:
    QVector<double> mGratingBeta;
    QVector<double> mBoundaryIndex;

    double mGratingPeroid;
    double mWaveGuideD;
    double mFreqParam;
    double mK;

    double mDielecParam1;
    double mDielecParam2;
    double mDielecParam3;

    QMutex mCoreMutex;

protected:
    SThread * parentThread;
    static int sCoreFunc(void * pParam, bool const & bRunning);
};

#endif // SCORECOMPUTATION_H
