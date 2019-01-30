#include "sGuidedMode.h"
#include "ui_sGuidedMode.h"

SGuidedMode::SGuidedMode(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SGuidedMode)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, false);
    settingDialog = new SSettingDialog();
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(ON_MENUBAR_ABOUT_CLICKED()));
    connect(ui->menuSetting, SIGNAL(aboutToShow()), this, SLOT(ON_MENUBAR_SETTING_CLICKED()));
}

SGuidedMode::~SGuidedMode()
{
    delete ui;
}


void SGuidedMode::ON_MENUBAR_ABOUT_CLICKED()
{
    QMessageBox::information(this, tr("About"),
                             tr("Guided Mode Calculation Tool\n"
                                "Developer:\tX.F.Zhou\nEmail:\txfzhou233@gmail.com\n"
                                "Copyright© 2018-2019 X.F.Zhou\nAll rights reserved."));
}

void SGuidedMode::ON_MENUBAR_SETTING_CLICKED()
{
    //占位，弹出设置窗口
    settingDialog->show();
}



