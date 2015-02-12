#ifndef WIDGET_H
#define WIDGET_H

#include <QLabel>
#include <QMenu>

class Widget : public QLabel
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    QRect getSelectionRect(){
        return selectionRect;
    }

signals:
    void pressed();
    void released();
protected:
//    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:

    bool selectionStarted;
    QRect selectionRect;
    QMenu contextMenu;



};

#endif // WIDGET_H
