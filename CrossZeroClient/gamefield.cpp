#include "gamefield.h"

Gamefield::Gamefield(QWidget * parent)
    : QTableWidget (parent)
{

}
//================================
Gamefield::Gamefield(){


}
//=======================================
Gamefield::~Gamefield()
{
    delete tmp;

}
//========================================
void Gamefield::mousePressEvent(QMouseEvent *event)
{

    if(event->button() == Qt::LeftButton && MainWindow::flag ){

        tmp = itemAt(event->pos());

        if(!tmp){
         tmp = new QTableWidgetItem();
         QPixmap pic;
         pic.load(MainWindow::imgpatch);

         tmp->setData(Qt::DecorationRole, pic);
         setItem(rowAt(event->pos().ry()), columnAt(event->pos().rx()), tmp);

         QString str = QString::number(rowAt(event->pos().ry())) + ',' + QString::number(columnAt(event->pos().rx()));
         MainWindow::socket->write("{\"type\":" "\"" +MainWindow::simbol.toUtf8()+ "\",\"cels\":" "\""+str.toUtf8()+ "\",\"status\":\"game\"}");

         MainWindow::cmotion();

         MainWindow::flag = false;
        }

    }
}
//==========================================
