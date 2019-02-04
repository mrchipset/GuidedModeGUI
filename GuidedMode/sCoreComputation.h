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

signals:
    void drawBeta(QVector<double>);

private:
    void getGrating(double const p);
    void getGrating();
    QString mLogger;
    SChartWidget * mChartWidget;

private:
    QVector<double> mGratingBeta;
    double mGratingPeroid;
    double mWaveGuideD;
    double mFreqency;

protected:
    SThread * parentThread;
    static int sCoreFunc(void * pParam, bool const & bRunning);
};

#endif // SCORECOMPUTATION_H
