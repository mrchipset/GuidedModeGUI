#ifndef SGUIDEDMODE_H
#define SGUIDEDMODE_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QMetaType>
#include <QDebug>

#include <qwt_plot.h>

#include "sSettingDialog.h"
#include "cGloabalParam.h"
#include "sChartWidget.h"
#include "sCoreComputation.h"
#include "Tools/sThread.h"

namespace Ui {
class SGuidedMode;
}

class SGuidedMode : public QMainWindow
{
    Q_OBJECT

public:
    explicit SGuidedMode(QWidget *parent = nullptr);
    ~SGuidedMode();

private:
    Ui::SGuidedMode *ui;
    SSettingDialog *settingDialog;
    SChartWidget *sPlotWidget;
    SCoreComputation *sCore;
    QString openedName;
    QMutex mPlotMutex;
    void loadParams();
    void readParams();

private slots:
    void ON_MENUBAR_ABOUT_CLICKED();//启动说明程序
//    void ON_MENUBAR_SETTING_CLICKED();

    void ON_MENUBAR_ACTION_OPEN();
    void ON_MENUBAR_ACTION_SAVE();
    void ON_MENUBAR_ACTION_SAVE_AS();
    void ON_PROCESS_START_CLICKED();
    void ON_PROCESS_TERMINATE_CLICKED();
    void ON_UPDATE_DRAWBETA();
    void ON_CALC_FINISHED();
    void ON_LOGGING();
    void ON_UPDATE_PROGRESS(int p);
};

#endif // SGUIDEDMODE_H
