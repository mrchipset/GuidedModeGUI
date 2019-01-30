#include "sSettingDialog.h"
#include "ui_SSettingDialog.h"

SSettingDialog::SSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SSettingDialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);//Close the help button.
    connect(ui->UI_PB_CONFIRM, SIGNAL(clicked()), this, SLOT(on_UI_PB_CONFIRM_CLICKED()));
}

SSettingDialog::~SSettingDialog()
{
    delete ui;
}

void SSettingDialog::on_UI_PB_CONFIRM_CLICKED()
{
    close();
}
