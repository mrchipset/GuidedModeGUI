#include "sGuidedMode.h"
#include "ui_sGuidedMode.h"

SGuidedMode::SGuidedMode(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SGuidedMode)
{
    ui->setupUi(this);
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
                             tr("波导传播模计算程序\n"
                                "开发者:\tX.F.Zhou\n联系:\txfzhou233@gmail.com"));
}

void SGuidedMode::ON_MENUBAR_SETTING_CLICKED()
{
    //占位，弹出设置窗口
    QMessageBox::information(this, tr("About"),
                             tr("波导传播模计算程序\n开发者:\tX.F.Zhou\n联系:\txfzhou233@gmail.com"));
}
