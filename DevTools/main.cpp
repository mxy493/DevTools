#include "devtools.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DevTools w;
    w.show();
    return a.exec();
}
