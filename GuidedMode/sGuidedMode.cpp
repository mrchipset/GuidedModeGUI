#include "sGuidedMode.h"
#include "ui_sGuidedMode.h"

SGuidedMode::SGuidedMode(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SGuidedMode)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, false);

    //Load the System Setting
    CGloabalParam::loadSysSetting();

    //Initilize the UI
    settingDialog = new SSettingDialog(this);
    sPlotWidget = SChartWidget::GetInstance(ui->UI_PLOT_WIDGET);
    sCore = new SCoreComputation;
    //Bind signal and slots.
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(ON_MENUBAR_ABOUT_CLICKED()));
    connect(ui->menuSetting, SIGNAL(aboutToShow()), settingDialog, SLOT(show()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(ON_MENUBAR_ACTION_OPEN()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(ON_MENUBAR_ACTION_SAVE()));
    connect(ui->actionSave_as, SIGNAL(triggered()), this, SLOT(ON_MENUBAR_ACTION_SAVE_AS()));
    connect(ui->UI_PB_START, SIGNAL(clicked()), this, SLOT(ON_PROCESS_START_CLICKED()));
    connect(ui->UI_PB_TERMINATE, SIGNAL(clicked()), this, SLOT(ON_PROCESS_TERMINATE_CLICKED()));
    connect(ui->actionPNG, SIGNAL(triggered()), sPlotWidget, SLOT(exportPNG()));
    connect(ui->actionTXT, SIGNAL(triggered()), sPlotWidget, SLOT(exportTXT()));
    connect(ui->actionCSV, SIGNAL(triggered()), sPlotWidget, SLOT(exportCSV()));
    //UI Update SLOTS
    connect(sCore,SIGNAL(drawBeta()), this, SLOT(ON_UPDATE_DRAWBETA()));
    connect(sCore, SIGNAL(calcFinished()), this, SLOT(ON_CALC_FINISHED()));
    connect(sCore, SIGNAL(logging()), this, SLOT(ON_LOGGING()));
    connect(sCore, SIGNAL(updateProgress(int)), this, SLOT(ON_UPDATE_PROGRESS(int)));
}


SGuidedMode::~SGuidedMode()
{
    //Save the system setting.
    CGloabalParam::saveSysSetting();
    sCore->stopProcess();
    delete sCore;
    delete ui;
}

void SGuidedMode::loadParams()
{
    CGloabalParam::GLOBAL_PARAM_MUTEX.lock();
    ui->UI_LE_EP1->setText(CGloabalParam::DIELEC_PARAM_1);
    ui->UI_LE_EP2->setText(CGloabalParam::DIELEC_PARAM_2);
    ui->UI_LE_EP3->setText(CGloabalParam::DIELEC_PARAM_3);
    ui->UI_LE_FREQ->setText(CGloabalParam::FREQ_PARAM_VALUE);
    ui->UI_RB_DIELECTRIC->setChecked(CGloabalParam::DIELEC_PARAM_TYPE);
    ui->UI_RB_REFRACTION->setChecked(!CGloabalParam::DIELEC_PARAM_TYPE);
    ui->UI_RB_FREQ->setChecked(CGloabalParam::FREQ_PARAM_TYPE);
    ui->UI_RB_LAMB->setChecked(!CGloabalParam::FREQ_PARAM_TYPE);
    ui->UI_LE_GRATING_P->setText(CGloabalParam::GEOMETRY_GRATING_P);
    ui->UI_LE_WAVEGUID_D->setText(CGloabalParam::GEOMETRY_WAVEGUIDE_D);
    CGloabalParam::GLOBAL_PARAM_MUTEX.unlock();
}

void SGuidedMode::readParams()
{
    CGloabalParam::GLOBAL_PARAM_MUTEX.lock();
    CGloabalParam::DIELEC_PARAM_1 = ui->UI_LE_EP1->text();
    CGloabalParam::DIELEC_PARAM_2 = ui->UI_LE_EP2->text();
    CGloabalParam::DIELEC_PARAM_3 = ui->UI_LE_EP3->text();
    CGloabalParam::FREQ_PARAM_VALUE = ui->UI_LE_FREQ->text();
    CGloabalParam::DIELEC_PARAM_TYPE = ui->UI_RB_DIELECTRIC->isChecked();
    CGloabalParam::FREQ_PARAM_TYPE = ui->UI_RB_FREQ->isChecked();
    CGloabalParam::GEOMETRY_GRATING_P = ui->UI_LE_GRATING_P->text();
    CGloabalParam::GEOMETRY_WAVEGUIDE_D = ui->UI_LE_WAVEGUID_D->text();
    CGloabalParam::GLOBAL_PARAM_MUTEX.unlock();
}

void SGuidedMode::ON_MENUBAR_ABOUT_CLICKED()
{
    QMessageBox::information(this, tr("About"),
                             tr("Guided Mode Calculation Tool\n"
                                "Developer:\tX.F.Zhou\nEmail:\txfzhou233@gmail.com\n"
                                "Copyright© 2018-2019 X.F.Zhou\nAll rights reserved."));
}

//void SGuidedMode::ON_MENUBAR_SETTING_CLICKED()
//{
//    settingDialog->show();
//}

void SGuidedMode::ON_MENUBAR_ACTION_OPEN()
{
    QString tmpName = QFileDialog::getOpenFileName(this, tr("Open Project"), QDir::currentPath(), tr("Project(*.gproj)"));

    if(!tmpName.isEmpty())
    {
        CGloabalParam::loadParam(tmpName);
        loadParams();
        openedName = tmpName;
        QFileInfo fileInfo(tmpName);
        this->setWindowTitle(QString("Guided Mode Calculator\t Opened: ").append(fileInfo.fileName()));
    }
}

void SGuidedMode::ON_MENUBAR_ACTION_SAVE()
{
    QString tmpName;
    if(openedName.isEmpty())
        tmpName = QFileDialog::getSaveFileName(this, tr("Save Project"), QDir::currentPath(), tr("Project(*.gproj)"));
    else
        tmpName = openedName;

    if(!tmpName.isEmpty())
    {
        readParams();
        CGloabalParam::saveParam(tmpName);
        openedName = tmpName;
        QFileInfo fileInfo(tmpName);
        this->setWindowTitle(QString("Guided Mode Calculator\t Opened: ").append(fileInfo.fileName()));
    }
}

void SGuidedMode::ON_MENUBAR_ACTION_SAVE_AS()
{

    QString tmpName = QFileDialog::getSaveFileName(this, tr("Save Project as"), QDir::currentPath(), tr("Project(*.gproj)"));

    if(!tmpName.isEmpty())
    {
        readParams();
        CGloabalParam::saveParam(tmpName);
        openedName = tmpName;
        QFileInfo fileInfo(tmpName);
        this->setWindowTitle(QString("Guided Mode Calculator\t Opened: ").append(fileInfo.fileName()));
    }
}

void SGuidedMode::ON_PROCESS_START_CLICKED()
{
    ui->UI_PB_START->setEnabled(false);
    ui->menuExport->setEnabled(false);
    readParams();
    mPlotMutex.lock();
    sPlotWidget->clearGraph();
    mPlotMutex.unlock();
    sCore->startProcess();
}

void SGuidedMode::ON_PROCESS_TERMINATE_CLICKED()
{
    ui->UI_PB_START->setEnabled(true);
    sCore->stopProcess();
}

void SGuidedMode::ON_UPDATE_DRAWBETA()
{
    mPlotMutex.lock();
    sPlotWidget->drawGratingLines();
    mPlotMutex.unlock();
}

void SGuidedMode::ON_CALC_FINISHED()
{
    ui->UI_PB_START->setEnabled(true);
    ui->menuExport->setEnabled(true);
    mPlotMutex.lock();
    sPlotWidget->setSamples();
    sPlotWidget->drawBoundaryIndex();
    mPlotMutex.unlock();
}

void SGuidedMode::ON_LOGGING()
{
    ui->UI_TB_LOGGER->append(sCore->Log());
}

void SGuidedMode::ON_UPDATE_PROGRESS(int p)
{
    ui->UI_PROGBAR_CALCULATION->setValue(p);
}
