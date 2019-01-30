#include "sSettingDialog.h"
#include "ui_SSettingDialog.h"

SSettingDialog::SSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SSettingDialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);//Close the help button.
}

SSettingDialog::~SSettingDialog()
{
    delete ui;
}
