#ifndef SCHARTWIDGET_H
#define SCHARTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QMutex>
#include <QVector>
#include <QPointF>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_picker.h>

class SChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SChartWidget(QWidget *parent = nullptr);
    ~SChartWidget();
public:
    void setSamples(QVector<QPointF> const& samples);
private:
    QwtPlot *mQwtPlot;
    QwtPlotCurve *mQwtScatter;
    QwtPlotZoomer *mQwtZoomer;
    QwtPlotPicker *mQwtPicker;
    QMutex mPlotMutex;

private:
    QVector<double> *mPointXQVec;
    QVector<double> *mPointYQVec;
signals:

public slots:
};

#endif // SCHARTWIDGET_H
