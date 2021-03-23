#include "watchwindow.h"
#include "ui_watchwindow.h"

WatchWindow::WatchWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WatchWindow),list()
{
    ui->setupUi(this);
    BasicInfo();
    modelInit();
    qDebug()<<"in";
    updateProcess();
    qDebug()<<"out";
    lastname="";
    ui->next->setVisible(false);

    QTimer* timer=new QTimer(this);
    for(int i=0;i<600;i++) CPURATE[i]=0;
    painter=new paint(ui->CPU,this->CPURATE);

    connect(timer,&QTimer::timeout,this,[=]{
        for(int i=0;i<599;i++){
            CPURATE[i]=CPURATE[i+1];
        }
        CPURATE[599]=CPU_CAL();
        painter->Repaint();
    });


    connect(timer,SIGNAL(timeout()),this,SLOT(updateTime()));

    timer->start(200);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(shutDown()));
    connect(ui->FIND,SIGNAL(clicked()),this,SLOT(find()));
    connect(ui->renew,SIGNAL(clicked()),this,SLOT(updateProcess()));//handle flash
    connect(ui->next,SIGNAL(clicked()),this,SLOT(next()));
    connect(ui->KILL,SIGNAL(clicked()),this,SLOT(kill()));

}
void WatchWindow::modelInit()
{
    model=new QStandardItemModel(2,4,this);
    model->setHeaderData(0,Qt::Horizontal,tr("pid"));
    model->setHeaderData(1,Qt::Horizontal,tr("name"));
    model->setHeaderData(2,Qt::Horizontal,tr("state"));
    model->setHeaderData(3,Qt::Horizontal,tr("ppid"));
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);;

}
void WatchWindow::BasicInfo()
{
    QFile hostnamefile("/proc/sys/kernel/hostname");//high release withou IO_
    QFile osfile("/proc/sys/kernel/ostype");
    QFile releasefile("/proc/sys/kernel/osrelease");
    QFile CPU("/proc/cpuinfo");
    QFile sys_time("/proc/uptime");

    hostnamefile.open(QIODevice::ReadOnly);
    sys_time.open(QIODevice::ReadOnly);
    osfile.open(QIODevice::ReadOnly);
    releasefile.open(QIODevice::ReadOnly);
    CPU.open(QIODevice::ReadOnly | QIODevice::Text);

    QDateTime boot_Time=QDateTime::currentDateTime();//boot time
    QString systime=sys_time.readAll();
    systime=systime.left(systime.indexOf('.'));
    long Time=systime.toLong();
    boot_Time=boot_Time.addSecs(0-Time);

    QString CPUline;//CPU info
    CPUline=CPU.readLine();
    while(!CPUline.isNull()){
        if(CPUline.left(10)==QString("model name").left(10)) break;
        CPUline=CPU.readLine();
    }
    CPUline=CPUline.right(CPUline.length()-CPUline.indexOf(':')-1);


    QString os=osfile.readAll();//os info
    os=os.left(os.length()-1);
    os+=" ";
    os+=releasefile.readAll();


    ui->label->setText("Hostname   :   "+QString(hostnamefile.readAll()));//display
    ui->label_2->setText("Boot Time   :   "+boot_Time.toString());
    ui->label_4->setText("System   :   "+os);
    ui->label_5->setText("CPU   :   "+CPUline);

    hostnamefile.close();//close related file
    osfile.close();
    releasefile.close();
    CPU.close();
    sys_time.close();
}
void WatchWindow::find()
{
    QString name=QString::fromStdString(ui->textEdit->toPlainText().toStdString());

    int i=ui->tableView->currentIndex().column();
    list=model->findItems(name,Qt::MatchExactly,i);
    if(list.size()){
        ite=list.begin();
        ui->tableView->selectRow(ite.operator*()->index().row());
     }
    if(list.size()>1) ui->next->setVisible(true);
    else ui->next->setVisible(false);
}
void WatchWindow::next()
{
    if(ite!=list.end()&&++ite!=list.end())
        ui->tableView->selectRow(ite.operator*()->index().row());
}
void WatchWindow::kill()
{

    QModelIndex i=ui->tableView->currentIndex();
    QModelIndex kill_i=i.sibling(i.row(),0);
    QString s="kill -9 "+kill_i.data().toString();
    if(ite!=list.end()) system(s.toUtf8());
}
void WatchWindow::shutDown()
{
    qDebug()<<"shutdown"<<endl;
    system("shutdown -h now");
}
double WatchWindow::CPU_CAL()
{
    unsigned long CPU[10];
    char cpuname[10];
    unsigned long totalCPUtime=0,totalCPUtime2=0;
    unsigned long idle;
    QFile Cpu_info("/proc/stat");

    Cpu_info.open(QIODevice::ReadOnly);//first open
    QString Cpu=Cpu_info.readLine();
    Cpu_info.close();

    sscanf(Cpu.toStdString().c_str(),"%s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",cpuname,&CPU[0],&CPU[1],&CPU[2],&CPU[3],&CPU[4],&CPU[5],&CPU[6],&CPU[7],&CPU[8],&CPU[9]);
    idle=CPU[3];
    for(int i=0;i<10;i++) totalCPUtime+=CPU[i];

    usleep(50000);//sample interval

    Cpu_info.open(QIODevice::ReadOnly);//second open
    Cpu=Cpu_info.readLine();
    Cpu_info.close();

    sscanf(Cpu.toStdString().c_str(),"%s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",cpuname,&CPU[0],&CPU[1],&CPU[2],&CPU[3],&CPU[4],&CPU[5],&CPU[6],&CPU[7],&CPU[8],&CPU[9]);
    idle=CPU[3]-idle;
    for(int i=0;i<10;i++) totalCPUtime2+=CPU[i];
    totalCPUtime=totalCPUtime2 -totalCPUtime;

    //qDebug("%lu    %lu",totalCPUtime,idle);
    return (totalCPUtime-idle)*1.0/totalCPUtime;
}
double WatchWindow::Mem_CAL()
{
    QFile mem_info("/proc/meminfo");
    mem_info.open(QIODevice::ReadOnly);

    QString mem=mem_info.readLine();//first line all memory
    mem=mem.right(mem.size()-mem.indexOf(':')-1).trimmed();
    mem=mem.left(mem.indexOf(' '));
    long memall=mem.toLong();

    mem=mem_info.readLine();
    mem=mem_info.readLine();//third line avaliable memory
    mem=mem.right(mem.size()-mem.indexOf(':')-1).trimmed();
    mem=mem.left(mem.indexOf(' '));
    mem_info.close();
    return (memall-mem.toLong())*1.0/memall;
}
double WatchWindow::Swap_CAL()
{
    QFile mem_info("/proc/meminfo");
    mem_info.open(QIODevice::ReadOnly);
    QString mem;
    for(int i=0;i<15;i++) mem=mem_info.readLine();
    mem=mem.right(mem.size()-mem.indexOf(':')-1).trimmed();
    mem=mem.left(mem.indexOf(' '));
    long swapall=mem.toLong();
    mem=mem_info.readLine();
    mem=mem.right(mem.size()-mem.indexOf(':')-1).trimmed();
    mem=mem.left(mem.indexOf(' '));
    mem_info.close();

    return (swapall-mem.toLong())*1.0/swapall;
}
void WatchWindow::updateTime()
{
       double Memeroy=Mem_CAL();
       double Swap=Swap_CAL();
       QFile sys_time("/proc/uptime");
       sys_time.open(QIODevice::ReadOnly);
       QString systime=sys_time.readAll();
       systime=systime.left(systime.indexOf('.'));//seconds
       long Time=systime.toLong();
       sys_time.close();
       ui->label_3->setText("Run   Time   :   "+QString::number(Time/3600)+":"+QString::number(Time%3600/60)+":"+QString::number(Time%3600%60));


       QDateTime Date_Time =QDateTime::currentDateTime();
       QString Time_system = Date_Time.toString("yyyy-MM-dd hh:mm:ss ddd");
       Time_system+="      Mem : ";
       Time_system+=QString::number(Memeroy);
       Time_system+="     CPU : ";
       Time_system+=QString::number(CPU_CAL());
       ui->Mem->setValue(Memeroy*100);
       ui->SWAP->setValue(Swap*100);




       ui->statusBar->showMessage(Time_system);
}
bool isDigital(QString src)
{
    QByteArray ba = src.toLatin1();//QString 转换为 char*
    const char *s = ba.data();
    while(*s && *s>='0' && *s<='9') s++;
    if (*s) return 0;
    else return 1;

}
void WatchWindow::updateProcess()
{
    model->clear();



    QString pid;
    QString name;
    QString state;
    QString ppid;
    int j=0;

    QDir qd("/proc");
    QStringList qentry=qd.entryList();
    for(int i=0;i<qentry.size();i++){
        QString a=qentry[i];
        if(isDigital(a)){//process
            QFile fentry("/proc/"+a+"/stat");
            fentry.open(QIODevice::ReadOnly);
            QString process=fentry.readAll();
            pid=process.left(process.indexOf(' '));
            process=process.right(process.size()-process.indexOf(' ')-1);
            name=process.mid(1,process.indexOf(' ')-2);
            process=process.right(process.size()-process.indexOf(' ')-1);
            state=process.left(1);
            process=process.right(process.size()-2);
            ppid=process.left(process.indexOf(' '));


            model->setItem(j,0,new QStandardItem(pid));
            model->setItem(j,1,new QStandardItem(name));
            model->setItem(j,2,new QStandardItem(state));
            model->setItem(j,3,new QStandardItem(ppid));

            j++;

            fentry.close();
        }
    }
    ui->tableView->setShowGrid(false);
    ui->tableView->verticalHeader()->setVisible(false);
}



WatchWindow::~WatchWindow()
{
    delete ui;
}


