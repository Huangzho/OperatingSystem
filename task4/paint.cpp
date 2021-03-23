#include "watchwindow.h"
paint::paint(QWidget *poi,double *pos):QWidget (poi),p(pos)
{
    setUpdatesEnabled(true);
    this->resize(600,180);
}
void paint::Repaint()
{
    this->update();
}
void paint::paintEvent(QPaintEvent *event)
{
    double width=this->width();
    QPainter painter(this);

    QLineF line[600];
    QPointF point[3];
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRect(rect());
    painter.setPen(QPen(Qt::black,1));
    line[0].setLine(0,20,width,20);
    line[1].setLine(0,90,width,90);
    line[2].setLine(0,160,width,160);
    line[3].setLine(width/2,20,width/2,160);
    painter.drawLines(line,4);
    point[0].setX(0);point[0].setY(20);
    point[1].setX(0);point[1].setY(90);
    point[2].setX(0);point[2].setY(160);

    painter.setPen(QPen(Qt::black,0.5));
    painter.drawText(point[0],"100%");
    painter.drawText(point[1],"50%");
    painter.drawText(point[2],"0%");
    painter.setPen(QPen(Qt::red,1));
    for(int i=0;i<599;i++)
        line[i].setLine(i,160-140*p[i],(i+1),160-140*p[i+1]);

    painter.drawLines(line,600);
}
