#ifndef WATCHWINDOW_H
#define WATCHWINDOW_H


#include <QMainWindow>
#include<QFile>
#include<QDebug>
#include<QDateTime>
#include<QTimer>
#include<QDir>// entry
#include<QTableWidgetItem>
#include <QListWidgetItem>
#include<QDateTime>
#include <unistd.h>
#include<QPainter>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>//table

namespace Ui {
class WatchWindow;
}
class paint:public QWidget
{
    Q_OBJECT
public:
    paint(QWidget *parent,double *p);
    void Repaint();
protected:
    void paintEvent(QPaintEvent *event);
private:
    QColor color=Qt::blue;
    QWidget *parent;
    double *p=nullptr;
};



class WatchWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WatchWindow(QWidget *parent = nullptr);
    ~WatchWindow();
public slots:
    void updateTime();
    void updateProcess();
    void shutDown();
    void find();
    void next();
    void kill();
public:
    double CPURATE[600];
private:
    Ui::WatchWindow *ui;
    paint* painter;
    QString lastname;
    QStandardItemModel* model;
    QList<QStandardItem*> list;
    QList<QStandardItem*>::iterator ite=nullptr;

private:
    double Mem_CAL();
    double CPU_CAL();
    double Swap_CAL();
    void BasicInfo();
    void modelInit();

};

#endif // WATCHWINDOW_H
