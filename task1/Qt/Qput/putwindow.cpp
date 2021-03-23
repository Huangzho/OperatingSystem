#include "putwindow.h"
#include "ui_putwindow.h"

PutWindow::PutWindow(QWidget *parent) :
    QMainWindow(parent),n(0),is_over(false),
    ui(new Ui::PutWindow)
{
    ui->setupUi(this);
    ui->label->setText("pid="+QString::number(getpid(),10));
    ui->label_2->setText("processing(KB):");
    ui->label_4->setText("running");


    QTimer*timer = new QTimer(this);//flash in a period time

    connect(timer,SIGNAL(timeout()),this,SLOT(Update()));
    timer->start(100);
}
void PutWindow::Update(void)
{
    if(is_over) ui->label_4->setText("finished");
    ui->label_3->setText(QString::number(n,10));
}


PutWindow::~PutWindow()
{
    delete ui;
}
