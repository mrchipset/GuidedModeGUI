#ifndef SGUIDEDMODE_H
#define SGUIDEDMODE_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

#include "sSettingDialog.h"
#include "cGloabalParam.h"
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
    QString openedName;
    void loadParams();
    void readParams();
private slots:
    void ON_MENUBAR_ABOUT_CLICKED();//启动说明程序
//    void ON_MENUBAR_SETTING_CLICKED();

    void ON_MENUBAR_ACTION_OPEN();
    void ON_MENUBAR_ACTION_SAVE();
    void ON_MENUBAR_ACTION_SAVE_AS();

};

#endif // SGUIDEDMODE_H
