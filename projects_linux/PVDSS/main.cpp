#include <QtGui/QApplication>
#include "PVDSS.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    PVDSS pvdss;
    pvdss.show();
    return app.exec();
}
