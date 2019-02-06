#include "sGuidedMode.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QVector<double>>("QVector<double>");
    SGuidedMode w;
    w.show();
    return a.exec();
}
