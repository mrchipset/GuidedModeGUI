#include "sChartWidget.h"

SChartWidget::SChartWidget(QWidget *parent) : QWidget(parent)
{
    //Initilize the QwtPlot and resize to fit the plot widget.
    mQwtPlot = new QwtPlot(parent);
    mQwtPlot->setCanvasBackground(QBrush(QColor(255,255,255)));
    mQwtPlot->move(0,0);
    mQwtPlot->resize(parent->width(),parent->height());
    mQwtPlot->setAxisTitle(QwtPlot::yLeft, tr("Beta"));
    mQwtPlot->setAxisTitle(QwtPlot::xBottom, tr("Omiga"));

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

SChartWidget::~SChartWidget()
{
    if(mPointXQVec != nullptr)
        delete mPointXQVec;
    mPointXQVec = nullptr;
    if(mPointYQVec != nullptr)
        delete mPointYQVec;
    mPointYQVec = nullptr;

    if(mQwtPicker != nullptr)
        delete mQwtPicker;
    mQwtPicker = nullptr;
    if(mQwtZoomer != nullptr)
        delete mQwtZoomer;
    mQwtZoomer = nullptr;
    if(mQwtScatter != nullptr)
        delete mQwtScatter;
    if(mQwtPlot != nullptr)
        delete mQwtPlot;
    mQwtPlot = nullptr;
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

