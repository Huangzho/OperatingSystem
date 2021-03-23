#ifndef COPYWINDOW_H
#define COPYWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <sys/types.h>
#include <unistd.h>

namespace Ui {
class CopyWindow;
}

class CopyWindow : public QMainWindow
{
    Q_OBJECT

public:
    long n;
    bool is_over;
    explicit CopyWindow(QWidget *parent = nullptr);
    ~CopyWindow();
public slots:
    void Update();
private:
    Ui::CopyWindow *ui;
};

#endif // COPYWINDOW_H
