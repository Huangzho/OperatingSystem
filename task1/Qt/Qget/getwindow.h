#ifndef GETWINDOW_H
#define GETWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <sys/types.h>
#include <unistd.h>


namespace Ui {
class GetWindow;
}

class GetWindow : public QMainWindow
{
    Q_OBJECT

public:
    long n;
    bool is_over;
    explicit GetWindow(QWidget *parent = nullptr);
    ~GetWindow();
public slots:
    void Update();


private:
    Ui::GetWindow *ui;
};

#endif // GETWINDOW_H
