#include "getwindow.h"
#include "ui_getwindow.h"

GetWindow::GetWindow(QWidget *parent) :
    QMainWindow(parent),is_over(false),n(0),
    ui(new Ui::GetWindow)
{
    ui->setupUi(this);

    ui->setupUi(this);
    ui->label->setText("pid="+QString::number(getpid(),10));
    ui->label_2->setText("processing(KB):");
    ui->label_4->setText("running");

    QTimer*timer = new QTimer(this);//flash in a period time
    timer->start(100);
    connect(timer,SIGNAL(timeout()),this,SLOT(Update()));
}

void GetWindow::Update()
{
    if(is_over) ui->label_4->setText("finished");
    ui->label_3->setText(QString::number(n,10));
}

GetWindow::~GetWindow()
{
    delete ui;
}
