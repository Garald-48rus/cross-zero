#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QObject>
#include <QtWidgets>
#include <QPixmap>
#include "mainwindow.h"


class Gamefield: public QTableWidget
{
    Q_OBJECT
public:
     Gamefield(QWidget * parent = 0);
     Gamefield();
    ~Gamefield();
private:
     void mousePressEvent(QMouseEvent *event) override;//переопределяю функцию нажатия кнопок миши

private:
     QTableWidgetItem* tmp;

};

#endif // GAMEFIELD_H
