#include "putwindow.h"
#include "putthread.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PutWindow w;
    w.setWindowTitle("put");
    w.show();
    w.move(1600,700);
    putThread P(atoi(argv[0]),atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),argv[4],w.n,w.is_over);
    P.start();


    return a.exec();
}
