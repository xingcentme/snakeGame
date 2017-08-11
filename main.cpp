#if MSC_VER >=1600
#pragma execution_character_set("UTF-8")
#endif

#include "Widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
