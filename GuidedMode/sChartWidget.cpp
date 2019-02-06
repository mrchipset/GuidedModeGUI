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
    mQwtPlot->setAxisTitle(QwtPlot::yLeft, tr("Frequency"));
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
    //Clear the grating vectical lines.
    //clearGratingLines();
    if(mPointXQVec != nullptr)
        delete mPointXQVec;
    mPointXQVec = nullptr;
    if(mPointYQVec != nullptr)
        delete mPointYQVec;
    mPointYQVec = nullptr;
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
    mPointXQVec->resize(samples.size());
    mPointYQVec->resize(samples.size());
    for(int i = 0; i < samples.size(); ++i)
    {
        (*mPointXQVec)[i] = samples[i].x();
        (*mPointYQVec)[i] = samples[i].y();
    }
    mQwtScatter->setSamples(mPointXQVec->data(),mPointYQVec->data(),mPointXQVec->size());
    mQwtPlot->replot();   
}

void SChartWidget::setSamples()
{
    mQwtScatter->setSamples(mPointXQVec->data(),mPointYQVec->data(),mPointXQVec->size());
    mQwtPlot->replot();
}

void SChartWidget::setPlotData(QVector<double> const& Beta, QVector<double> const& F)
{
    mPointXQVec->resize(Beta.size());
    mPointYQVec->resize(F.size());
    memcpy(mPointXQVec->data(), Beta.data(), sizeof(double)*static_cast<unsigned int>(Beta.size()));
    memcpy(mPointYQVec->data(), F.data(), sizeof(double)*static_cast<unsigned int>(F.size()));
}

void SChartWidget::addGratingLine(double const x)
{
    QwtPlotMarker * pLine = new QwtPlotMarker;
    pLine->setLineStyle(QwtPlotMarker::VLine);
    pLine->setLinePen(QPen(Qt::blue, 1, Qt::DashDotDotLine));
    pLine->setXValue(x);
    pLine->attach(mQwtPlot);
    mGratingLines.append(pLine);
    mQwtPlot->replot();
}

void SChartWidget::clearGratingLines()
{

    for(auto var : mGratingLines)
    {
        if(var)
            delete var;
        var = nullptr;
    }
    mGratingLines.clear();

}

void SChartWidget::setGratingLines(QVector<double> const & beta)
{

    mBetaQVec.clear();
    mBetaQVec.resize(beta.size());
    memcpy(mBetaQVec.data(),beta.data(),sizeof(double)*static_cast<unsigned int>(mBetaQVec.size()));

}

void SChartWidget::drawGratingLines()
{

    //Clear Existed Lines
    clearGratingLines();
    //Add new Lines
    for(auto var : mBetaQVec)
        addGratingLine(var);

}

void SChartWidget::setBoundaryIndex(QVector<double> const& index)
{
    mBoundaryIndex.clear();
    mBoundaryIndex.resize(index.size());
    memcpy(mBoundaryIndex.data(), index.data(), sizeof(double)*static_cast<unsigned int>(index.size()));
}
