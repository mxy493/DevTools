#include <QtWidgets/QApplication>

#include "devtools.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    load_config();

    QApplication a(argc, argv);
    DevTools w;
    w.show();
    return a.exec();
}
