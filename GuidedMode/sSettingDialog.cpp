#include "sSettingDialog.h"
#include "ui_SSettingDialog.h"

SSettingDialog::SSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SSettingDialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);//Close the help button.

    //load the saved system settings
    ui->UI_CB_THREADS->setCurrentIndex(CGloabalParam::COCURRENT);
    ui->UI_CB_LANG->setCurrentIndex(CGloabalParam::SYSTEM_LANG);
    ui->UI_LE_SAVEPTS->setText(QString::number(CGloabalParam::SAVE_DATA_PTS));
    ui->UI_CB_FREQ_ACCURACY->setCurrentIndex(CGloabalParam::FREQ_ACCURACY);
    ui->UI_CB_ZERO_THRESHOLD->setCurrentIndex(CGloabalParam::ZERO_THRESHOLD);
    //bind signal and slots
    connect(ui->UI_PB_CONFIRM, SIGNAL(clicked()), this, SLOT(ON_UI_PB_CONFIRM_CLICKED()));
}

SSettingDialog::~SSettingDialog()
{
    delete ui;
}

void SSettingDialog::ON_UI_PB_CONFIRM_CLICKED()
{
    CGloabalParam::GLOBAL_PARAM_MUTEX.lock();
    CGloabalParam::FREQ_ACCURACY = ui->UI_CB_FREQ_ACCURACY->currentIndex();
    CGloabalParam::COCURRENT = ui->UI_CB_THREADS->currentIndex();
    CGloabalParam::SAVE_DATA_PTS = ui->UI_LE_SAVEPTS->text().toInt();
    CGloabalParam::SYSTEM_LANG = ui->UI_CB_LANG->currentIndex();
    CGloabalParam::ZERO_THRESHOLD = ui->UI_CB_ZERO_THRESHOLD->currentIndex();
    CGloabalParam::GLOBAL_PARAM_MUTEX.unlock();
    close();
}


