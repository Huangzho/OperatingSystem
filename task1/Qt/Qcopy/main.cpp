#include "copywindow.h"
#include "copythread.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CopyWindow w;
    w.show();
    w.move(900,700);
    w.setWindowTitle("copy");

    copyThread P(atoi(argv[0]),atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),w.n,w.is_over);//muti thread
    P.start();


    return a.exec();
}
