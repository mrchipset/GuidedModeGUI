#ifndef SSETTINGDIALOG_H
#define SSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class SSettingDialog;
}

class SSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SSettingDialog(QWidget *parent = nullptr);
    ~SSettingDialog();

private:
    Ui::SSettingDialog *ui;
private slots:
    void on_UI_PB_CONFIRM_CLICKED();
};

#endif // SSETTINGDIALOG_H
