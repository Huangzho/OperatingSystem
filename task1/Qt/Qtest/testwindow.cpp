#include "testwindow.h"
#include "ui_testwindow.h"

TestWindow::TestWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);
    ui->setupUi(this);
    ui->label->setText("pid="+QString::number(getpid(),10));
    ui->label_2->setText("running");
    QTimer* timer=new QTimer(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(Update()));
    timer->start(100);
}
void TestWindow::Update()
{

    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy_MM_dd dddd hh:mm:ss");
    ui->label_2->setText(str);
}

TestWindow::~TestWindow()
{
    delete ui;
}
