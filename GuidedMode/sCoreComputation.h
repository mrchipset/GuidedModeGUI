#ifndef SCORECOMPUTATION_H
#define SCORECOMPUTATION_H

#include <QVector>
#include <armadillo>
#include "sChartWidget.h"

class sCoreComputation
{
public:
    sCoreComputation();
private:
    QVector<double> getGrating(double p);
    SChartWidget * mChartWidget;
};

#endif // SCORECOMPUTATION_H
