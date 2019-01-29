#ifndef SGUIDEDMODE_H
#define SGUIDEDMODE_H

#include <QMainWindow>
#include <QMessageBox>
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

private slots:
    void ON_MENUBAR_ABOUT_CLICKED();//启动说明程序
    void ON_MENUBAR_SETTING_CLICKED();

};

#endif // SGUIDEDMODE_H
