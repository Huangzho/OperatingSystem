#include "getwindow.h"
#include "getthread.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    GetWindow w;
    w.show();
    w.move(200,700);
    w.setWindowTitle("get");

    getThread P(atoi(argv[0]),atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),argv[4],w.n,w.is_over);//muti thread
    P.start();


    a.exec();

    return 0;
}
