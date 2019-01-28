#ifndef SGUIDEDMODE_H
#define SGUIDEDMODE_H

#include <QMainWindow>

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
};

#endif // SGUIDEDMODE_H
