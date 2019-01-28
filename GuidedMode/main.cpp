#include "sGuidedMode.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SGuidedMode w;
    w.show();

    return a.exec();
}
