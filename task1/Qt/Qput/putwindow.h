#ifndef PUTWINDOW_H
#define PUTWINDOW_H

#include <QMainWindow>
#include<QTimer>
#include <sys/types.h>
#include <unistd.h>


namespace Ui {
class PutWindow;
}

class PutWindow : public QMainWindow
{
    Q_OBJECT

public:
    long n;
    bool is_over;
    explicit PutWindow(QWidget *parent = nullptr);
    ~PutWindow();

private:
    Ui::PutWindow *ui;
public slots:
    void Update(void);
};

#endif // PUTWINDOW_H
