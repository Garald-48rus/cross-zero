#include "start.h"
#include "ui_start.h"

Start::Start(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Start)
{
    ui->setupUi(this);
    ui->pushButton->setStyleSheet("color:green");
}
//=================================================
Start::~Start()
{
    delete ui;
}
//==================================================
void Start::on_toolButton_clicked()
{

    imgadr = QFileDialog::getOpenFileName(0, "Открыть изображение",".../","*.png *.jpg *.bmp *.JPG");
     pic = new  QPixmap(imgadr);
    *pic = pic->scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio);
     ui->label->setPixmap(*pic);

}
//=====================================================

void Start::on_pushButton_clicked()
{

    if(imgadr == ""){
        ui->label_2->setText("Вы не выбрали фото");
    }
    else {

        emit _addData();
        this->close();
    }


}
//=============================================================
