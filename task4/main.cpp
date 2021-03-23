#include "watchwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WatchWindow w;
    w.show();
    w.move(600,400);
    w.setFixedSize(800,500);

    return a.exec();
}
