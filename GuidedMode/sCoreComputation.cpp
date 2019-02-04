#include "sCoreComputation.h"

sCoreComputation::sCoreComputation()
{
    //Initilize the Qwt Plot Singleton Instance.
    mChartWidget = SChartWidget::GetInstance();
}
