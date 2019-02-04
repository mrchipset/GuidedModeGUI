#include "sChartWidget.h"


SChartWidget * SChartWidget::mInstance = nullptr;
SChartWidget::GC SChartWidget::Gc;
QMutex SChartWidget::mInstanceMutex;
SChartWidget * SChartWidget::GetInstance(QWidget *parent)
{
    mInstanceMutex.lock();
    if(SChartWidget::mInstance == nullptr)
        SChartWidget::mInstance = new SChartWidget(parent);
    mInstanceMutex.unlock();
    return SChartWidget::mInstance;
}

SChartWidget::SChartWidget(QWidget *parent) : QWidget(parent)
{
    //Initilize the QwtPlot and resize to fit the plot widget.
    mQwtPlot = new QwtPlot(parent);
    mQwtPlot->setCanvasBackground(QBrush(QColor(255,255,255)));
    mQwtPlot->move(0,0);
    mQwtPlot->resize(parent->width(),parent->height());
    mQwtPlot->setAxisTitle(QwtPlot::yLeft, tr("Omiga"));
    mQwtPlot->setAxisTitle(QwtPlot::xBottom, tr("Beta"));
    mQwtPlot->setTitle(tr("Dispersion Curve Chart"));
    mQwtPlot->setAxisAutoScale(QwtPlot::yLeft);
    mQwtPlot->setAxisAutoScale(QwtPlot::xBottom);
    mQwtPlot->setAxisScale(QwtPlot::yLeft,0,1e12);
    mQwtPlot->setAxisScale(QwtPlot::xBottom,0,1e5);

    //Initilize the Qwt Plot Curves
    mQwtScatter = new QwtPlotCurve(tr("Scatter"));
    mQwtScatter->attach(mQwtPlot);
    mQwtScatter->setStyle(QwtPlotCurve::Dots);

    //Configure the zoomer and picker for plot
    mQwtZoomer = new QwtPlotZoomer(mQwtPlot->canvas(), true);
    mQwtPicker = new QwtPlotPicker(mQwtPlot->canvas());
    mQwtPicker->setTrackerMode(QwtPlotPicker::AlwaysOn);

    mPointXQVec = new QVector<double>();
    mPointYQVec = new QVector<double>();
}


void SChartWidget::sClean()
{
    mPlotMutex.lock();
    //Clear the grating vectical lines.
    //clearGratingLines();
    if(mPointXQVec != nullptr)
        delete mPointXQVec;
    mPointXQVec = nullptr;
    if(mPointYQVec != nullptr)
        delete mPointYQVec;
    mPointYQVec = nullptr;
    mPlotMutex.unlock();

}

SChartWidget::~SChartWidget()
{
//    //Clear the grating vectical lines.
//    clearGratingLines();

    if(mPointXQVec != nullptr)
        delete mPointXQVec;
    mPointXQVec = nullptr;
    if(mPointYQVec != nullptr)
        delete mPointYQVec;
    mPointYQVec = nullptr;

//    if(mQwtPicker != nullptr)
//        delete mQwtPicker;
//    mQwtPicker = nullptr;
//    if(mQwtZoomer != nullptr)
//        delete mQwtZoomer;
//    mQwtZoomer = nullptr;
//    if(mQwtScatter != nullptr)
//        delete mQwtScatter;
//    if(mQwtPlot != nullptr)
//        delete mQwtPlot;
//    mQwtPlot = nullptr;
}

void SChartWidget::setSamples(QVector<QPointF> const& samples)
{
    mPlotMutex.lock();
    mPointXQVec->resize(samples.size());
    mPointYQVec->resize(samples.size());
    for(int i = 0; i < samples.size(); ++i)
    {
        (*mPointXQVec)[i] = samples[i].x();
        (*mPointYQVec)[i] = samples[i].y();
    }
    mQwtScatter->setSamples(mPointXQVec->data(),mPointYQVec->data(),mPointXQVec->size());
    mQwtPlot->replot();
    mPlotMutex.unlock();
}

void SChartWidget::addGratingLine(double const x)
{
    mPlotMutex.lock();
    QwtPlotMarker * pLine = new QwtPlotMarker;
    pLine->setLineStyle(QwtPlotMarker::VLine);
    pLine->setLinePen(QPen(Qt::blue, 1, Qt::DashDotDotLine));
    pLine->setXValue(x);
    pLine->attach(mQwtPlot);
    mGratingLines.append(pLine);
    mQwtPlot->replot();
    mPlotMutex.unlock();
}

void SChartWidget::clearGratingLines()
{
    mPlotMutex.lock();
    for(auto var : mGratingLines)
    {
        if(var)
            delete var;
        var = nullptr;
    }
    mGratingLines.clear();
    mPlotMutex.unlock();
}
