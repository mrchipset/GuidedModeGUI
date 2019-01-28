#include "sGuidedMode.h"
#include "ui_sGuidedMode.h"

SGuidedMode::SGuidedMode(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SGuidedMode)
{
    ui->setupUi(this);
}

SGuidedMode::~SGuidedMode()
{
    delete ui;
}
