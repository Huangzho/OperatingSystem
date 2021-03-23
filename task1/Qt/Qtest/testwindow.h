#ifndef TESTWINDOW_H
#define TESTWINDOW_H
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/wait.h>
#include<unistd.h>

#include<QTimer>
#include <QDateTime>
#include <QMainWindow>

namespace Ui {
class TestWindow;
}

class TestWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestWindow(QWidget *parent = nullptr);
    ~TestWindow();
public slots:
    void Update();

private:
    Ui::TestWindow *ui;
};

#endif // TESTWINDOW_H
