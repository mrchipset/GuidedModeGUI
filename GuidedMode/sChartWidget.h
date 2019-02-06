#ifndef SCHARTWIDGET_H
#define SCHARTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QMutex>
#include <QVector>
#include <QList>
#include <QPointF>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_picker.h>

class SChartWidget : public QWidget
{
    Q_OBJECT
public:
    static SChartWidget * GetInstance(QWidget *parent = nullptr);
    ~SChartWidget();
private:
    explicit SChartWidget(QWidget *parent = nullptr);
    static SChartWidget * mInstance;
    class GC{
        public:
            ~GC(){
                SChartWidget::mInstanceMutex.lock();
                if(SChartWidget::mInstance != nullptr)
                    SChartWidget::mInstance->sClean();//Crashed when call deconstructor.
                SChartWidget::mInstanceMutex.unlock();
                SChartWidget::mInstance = nullptr;
            }
    };
    static GC Gc;

public:
    void setSamples(QVector<QPointF> const& samples);
    void setSamples();
    void clearGraph(){
        mPointXQVec->clear();
        mPointYQVec->clear();
        setSamples();
        clearGratingLines();
    }
    void addGratingLine(double const x);
    void setGratingLines(QVector<double> const & beta);
    void setBoundaryIndex(QVector<double> const & index);
    void drawGratingLines();
    void clearGratingLines();
    void setPlotData(QVector<double> const& Beta, QVector<double> const& F);

    void sClean();
private:
    QwtPlot *mQwtPlot;
    QwtPlotCurve *mQwtScatter;
    QwtPlotZoomer *mQwtZoomer;
    QwtPlotPicker *mQwtPicker;
    static QMutex mInstanceMutex;

private:
    QVector<double> *mPointXQVec;
    QVector<double> *mPointYQVec;
    QVector<double> mBetaQVec;
    QVector<double> mBoundaryIndex;
    QList<QwtPlotMarker*> mGratingLines;
signals:

public slots:
};

#endif // SCHARTWIDGET_H
