#include "widget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QAction>
#include <QFileDialog>

#include <iostream>

Widget::Widget(QWidget *parent)
    : QLabel(parent)
{
    selectionStarted=false;
}

Widget::~Widget()
{

}
/*
void Widget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QLabel::paintEvent(e);
    painter.setPen(QPen(QBrush(QColor(0,0,0,180)),1,Qt::DashLine));
    painter.setBrush(QBrush(QColor(255,255,255,120)));

    painter.drawRect(selectionRect);
}
*/
void Widget::mousePressEvent(QMouseEvent *e)
{
    emit(pressed());
    if (e->button()==Qt::RightButton)
    {
        if (selectionRect.contains(e->pos())) contextMenu.exec(this->mapToGlobal(e->pos()));
    }
    else
    {
    selectionStarted=true;
    selectionRect.setTopLeft(e->pos());
    selectionRect.setBottomRight(e->pos());
    }

    std::cout << "test test";

}

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    if (selectionStarted)
    {
        selectionRect.setBottomRight(e->pos());
        repaint();
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    emit(released());
    selectionStarted=false;

}
