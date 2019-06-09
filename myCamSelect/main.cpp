#include "widget.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.setWindowTitle("Rover-DHM-01-Beta");
    w.show();

    return a.exec();
}
