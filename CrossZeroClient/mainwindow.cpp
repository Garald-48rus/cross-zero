#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap bkgnd(":/img/dekstop.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    mStart = new Start();

    ui->m_showEmojiListButton->setCursor(Qt::PointingHandCursor);
    ui->m_showEmojiListButton->setIcon(QIcon(":/emoji/emoji_u1f642.png"));
    ui->m_showEmojiListButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");

    ui->button_yes->setStyleSheet("QPushButton { border: none; padding: 0px; }");

    m_emojiListWidget = new QListWidget();
    m_emojiListWidget->setWindowFlags(Qt::ToolTip);
    m_emojiListWidget->setLayoutMode(QListView::Batched);
    m_emojiListWidget->setViewMode(QListView::IconMode);
    m_emojiListWidget->setGridSize(QSize(32,32));
    m_emojiListWidget->setWrapping(true);
    m_emojiListWidget->installEventFilter(this);



    ui->pushButton_2->setStyleSheet("color:red");
    ui->pushButton_3->setStyleSheet("color:blue");
    ui->pushButton->setStyleSheet("color:green");
    ui->button_yes->setStyleSheet("color:darkGreen");
    ui->button_no->setStyleSheet("color:red");
    ui->label_continue->setStyleSheet("color:blue");
    ui->label_7->setStyleSheet("color:darkMagenta");
    ui->label_6->setStyleSheet("color:darkBlue");
    ui->lineEdit->setStyleSheet("color:darkGreen");

    ui->textEdit->setFont(QFont(QStringLiteral("A Font Supporting Emoji")));

    socket = new QTcpSocket(this);
    Effect = new QGraphicsBlurEffect();

    connect(socket, SIGNAL(readyRead()),this, SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(sockDisc()));
    socket->connectToHost("127.0.0.1", 5555);

      label_6 = ui->label_5;

      flemoji = true;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mStart;
    delete mItem;
    delete socket;
    delete label_6;
    delete  Effect;
}

//=========================================
void MainWindow::on_pushButton_clicked()
{   
   //отправка имени игрока

    if(!ui->lineEdit ->text().isEmpty()){
     QObject::connect(mStart, &Start::_addData, this, &MainWindow::_sendData);
     mStart->show();
     socket->write("{\"type\":\"info\",\"name\":" "\"" +ui->lineEdit->text().toUtf8()+ "\"""}");
     ui->pushButton->setStyleSheet("");
     ui->pushButton->setEnabled(false);
     ui->label_4->setText("Ваше имя/ник");
     ui->lineEdit->setReadOnly(true);
    }

}
//===========================================
void MainWindow::_sendData()
{
//отправка фото
    QBuffer* buffer;
    QByteArray itog;

    pic.load(mStart->imgadr, "jpg");
    pic = pic.scaled(ui->label_3->width(), ui->label_3->height(), Qt::KeepAspectRatio);
    buffer = new QBuffer(&itog);
    buffer->open(QIODevice::WriteOnly);
    pic.save(buffer,"jpg");
    socket->waitForBytesWritten(500);
    socket->write(itog);

    delete buffer;
}
//============================================
void MainWindow::sockReady()
{

    if(socket->waitForConnected(500)){

        Data = socket->readAll();
        doc = QJsonDocument::fromJson(Data, &docError);
//=================================================================================
        //статус подключения
        if (docError.errorString()=="no error occurred"){

            if(doc.object().value("type").toString() == "connected"
                    && doc.object().value("status").toString() == "yes"){

               ui->label->setText("Подключен");
               ui->pushButton->setEnabled(true);

            }
//=====================================================================================
            else if (doc.object().value("type").toString() == "info") {
               //получение ника соперника
                ui->label_2->setStyleSheet("color:red");
                ui->label_2->setText("Соперник: " + doc.object().value("name").toString());
                name = doc.object().value("name").toString();

            }
//==================================================================================
            else if (doc.object().value("type").toString() == "symbol")
            {
                   //рандомное рапределение символов игры

                 if(doc.object().value("value").toString() == "x")
                 {

                    imgpatch = ":/img/x.png";
                    pic.load(imgpatch);
                    pic = pic.scaled(ui->tableWidget->width(), ui->tableWidget->height(), Qt::KeepAspectRatio);
                    mItem = new QTableWidgetItem();
                    mItem->setData(Qt::DecorationRole, pic);
                    ui->tableWidget->setItem(0,0, mItem);
                    ui->label_5->setText("Ваш ход");
                    simbol = "x";
                    flag = true;
                    ui->label_6->setText("Счет: " + QString::number(win) + " : " + QString::number(loose));
                    ui->label_7->setText("Раунд: "+QString::number(round));
                 }

                 else if (doc.object().value("value").toString() == "o") {

                      imgpatch = ":/img/O.png";
                      pic.load(imgpatch);
                      pic = pic.scaled(ui->tableWidget->width(), ui->tableWidget->height(), Qt::KeepAspectRatio);
                      mItem = new QTableWidgetItem();
                      mItem->setData(Qt::DecorationRole, pic);
                      ui->tableWidget->setItem(0,0, mItem);
                      ui->label_5->setText("Ход соперника");
                      simbol = "o";
                      ui->label_6->setText("Счет: " + QString::number(win) + " : " + QString::number(loose));
                      ui->label_7->setText("Раунд: "+QString::number(round));
                 }

            }
//=================================================================================
            else if (doc.object().value("status").toString() == "game") {
               //игра ходы игроков
                if(doc.object().value("type").toString() == "x")
                {

                 QString str = doc.object().value("cels").toString();

                 mItem = new QTableWidgetItem();
                 int row = str.at(0).unicode() - '0';
                 int col = str.at(2).unicode() - '0';
                 pic.load(":/img/x.png");
                 mItem->setData(Qt::DecorationRole, pic);
                 ui->tableWidget_2->setItem(row, col, mItem);
                 label_6->setStyleSheet("color:dark");
                 ui->label_5->setText("Ваш ход");
                 flag = true;
                }

                if(doc.object().value("type").toString() == "o")
                {

                 QString str = doc.object().value("cels").toString();

                 auto row = str.at(0).unicode() - '0';
                 auto col = str.at(2).unicode() - '0';
                 pic.load(":/img/O.png");
                 mItem = new QTableWidgetItem();
                 mItem->setData(Qt::DecorationRole, pic);
                 ui->tableWidget_2->setItem(row, col, mItem);
                 label_6->setStyleSheet("color:dark");
                 ui->label_5->setText("Ваш ход");
                 flag = true;
                }               


            }
//=============================================================================
            else if (doc.object().value("type").toString() == "win") {
               //если выграл
                flag = false;

                auto row{0};
                auto col{0};
                label_6->setStyleSheet("color:green");
                label_6->setText("Вы выграли раунд");

                if(simbol == 'o'){ pic.load(":/img/O.png");}
                else if (simbol == 'x') {pic.load(":/img/x.png");}

                QString str = doc.object().value("cels").toString();

                for (auto i = 0; i < str.size(); i++) {

                  if(str.at(i) == '.') {mItem = new QTableWidgetItem();
                  mItem->setBackground(Qt::green); mItem->setData(Qt::DecorationRole, pic);
                  ui->tableWidget_2->setItem(row, col, mItem); ;continue;}

                  row = str.at(i).unicode() - '0';
                  i+=2;
                  col = str.at(i).unicode() - '0';

                }
                win++;
                round++;
                //если мгра еще не зоканчена
                if(win < 3){
                  radius -= 20;
                  Effect->setBlurRadius(radius);
                  ui->label_3->setGraphicsEffect(Effect);
                  ui->label_6->setText("Счет: " + QString::number(win) + " : " + QString::number(loose));
                  ui->pushButton_4->setStyleSheet("color:green");
                  ui->pushButton_4->setEnabled(true);
                }
                else {
                    radius = 0;
                    Effect->setBlurRadius(radius);
                    ui->label_3->setGraphicsEffect(Effect);
                    ui->label_6->setText("Счет: " + QString::number(win) + " : " + QString::number(loose));
                    label_6->setStyleSheet("color:green");
                    label_6->setText("Матч выгран");

                    ui->label_continue->setEnabled(true);
                    ui->button_no->setEnabled(true);
                    ui->button_yes->setEnabled(true);
                    ui->label_continue->setText("Еще?");
                    ui->button_yes->setText("Да");
                    ui->button_no->setText("Нет");

                    flag = false;
                }

            }
//====================================================================================
            else if (doc.object().value("type").toString() == "loser") {
                //если проиграл
                flag = false;

                auto row{0};
                auto col{0};
                label_6->setStyleSheet("color:red");
                label_6->setText("Вы проиграли раунд");

                if(simbol == 'o'){ pic.load(":/img/x.png");}
                else if (simbol == 'x') {pic.load(":/img/O.png");}

                QString str = doc.object().value("cels").toString();
                for (auto i = 0; i < str.size(); i++) {

                 if(str.at(i) == '.'){mItem = new QTableWidgetItem();
                     mItem->setBackgroundColor(Qt::red); mItem->setData(Qt::DecorationRole, pic);
                     ui->tableWidget_2->setItem(row, col, mItem); continue;}

                  row = str.at(i).unicode() - '0';
                  i += 2;
                  col = str.at(i).unicode() - '0';
                }

                loose++;
                round++;

                if(loose < 3 && win < 3){
                 ui->label_6->setText("Счет: " + QString::number(win) + " : " + QString::number(loose));
                 ui->pushButton_4->setStyleSheet("color:green");
                 ui->pushButton_4->setEnabled(true);
                }
                else {
                    ui->label_6->setText("Счет: " + QString::number(win) + " : " + QString::number(loose));
                    label_6->setStyleSheet("color:red");
                    label_6->setText("Матч проигран");

                    ui->label_continue->setEnabled(true);
                    ui->button_no->setEnabled(true);
                    ui->button_yes->setEnabled(true);
                    ui->label_continue->setText("Еще?");
                    ui->button_yes->setText("Да");
                    ui->button_no->setText("Нет");

                    flag = false;
                }
            }
//===========================================================================================
            else if(doc.object().value("type").toString() == "draw") {
                //если ничья
                round++;
                ui->label_7->setText("Раунд: "+QString::number(round));
                ui->pushButton_4->setStyleSheet("color:green");
                ui->pushButton_4->setEnabled(true);
                label_6->setStyleSheet("color:dark");
                ui->label_5->setText("Ничья");
            }
//==================================================================================
            else if (doc.object().value("type").toString() == "continue") {
               //продолжение игры
                if(simbol == 'x'){
                  label_6->setStyleSheet("color:dark");
                  ui->label_5->setText("Ваш ход");
                  flag = true;
                }
            }
//==================================================================================
            else if (doc.object().value("type").toString() == "chat") {
              //получение сообщений в чат
                ui->textEdit->setTextColor(Qt::red);
               if(doc.object().value("emoji").toString() == "false"){
                ui->textEdit->append(name + ": " + doc.object().value("text").toString());
               }
               else if (doc.object().value("emoji").toString() == "true") {

                   ui->textEdit->append(name + ": " + doc.object().value("text").toString());
                   ui->textEdit->insertHtml(QString("<img src=\"%1\" width=\"32\" height=\"32\">").
                                            arg(QString(doc.object().value("patchemj").toString()).replace("22x22", "14x14")));
               }
            }
//=========================================================================
           //получение информации о начале новой игры
            else if (doc.object().value("type").toString() == "yes") {

                mStart->show();

            }
            else if (doc.object().value("type").toString() == "no") {
                label_6->setStyleSheet("color:red");
                label_6->setText("Ваш соперник отключился");
            }
        }
//=============================================================================
        else if (docError.errorString()!="no error occurred" && !Data.isEmpty()) {
            //получение фото соперника
        QBuffer imagebuffer;
        imagebuffer.open(QIODevice::ReadWrite);
        imagebuffer.write(Data);

        pic.loadFromData(imagebuffer.buffer());


        ui->label_3->setPixmap(pic);
        Effect->setBlurRadius(radius);

        ui->label_3->setGraphicsEffect(Effect);

        if(ui->label_3->text().isEmpty()){

           socket->write("{\"type\":\"ready\"}");
        }

        ui->pushButton_3->setEnabled(true);
        ui->m_showEmojiListButton->setEnabled(true);
      }

    }
}
//===================================
void MainWindow::sockDisc(){

    socket->deleteLater();
}
//=========================================
void MainWindow::cmotion()
{

   label_6->setStyleSheet("color:dark");
   label_6->setText("Ход соперника");

}

//=============================

void MainWindow::on_pushButton_2_clicked()
{

    std::exit(0);
}
//=================================================
void MainWindow::on_pushButton_3_clicked()
{
    //отправка сообщений в чате
   if(!ui->textEdit_chatMessage->toPlainText().isEmpty())
   {
   ui->textEdit->setTextColor(Qt::blue);
   ui->textEdit->append("Вы: " + ui->textEdit_chatMessage->toPlainText());

   if(emjsend){

    ui->textEdit->insertHtml(QString("<img src=\"%1\" width=\"32\" height=\"32\">").arg(QString(emjpatch).replace("22x22", "14x14")));

    socket->write("{\"type\":\"chat\", \"text\": " "\"" +ui->textEdit_chatMessage->toPlainText().toUtf8()+ "\","
" \"emoji\":\"true\", \"patchemj\": " "\""+emjpatch.toUtf8() + "\"}");

    emjsend = false;
   }
   else {

       socket->write("{\"type\":\"chat\", \"text\": " "\"" +ui->textEdit_chatMessage->toPlainText().toUtf8()+ "\", "
"\"emoji\":\"false\"}");
   }

   ui->textEdit_chatMessage->clear();
  }
}
//==========================================
void MainWindow::draw()
{
  //обмен символами новый раунд
    ui->tableWidget->clear();
    ui->tableWidget_2->clear();


  if(simbol == 'x'){
    simbol = 'o';
    imgpatch = ":/img/O.png";
    pic.load(imgpatch);
    pic = pic.scaled(ui->tableWidget->width(), ui->tableWidget->height(), Qt::KeepAspectRatio);
    mItem = new QTableWidgetItem();
    mItem->setData(Qt::DecorationRole, pic);
    ui->tableWidget->setItem(0,0, mItem);
    label_6->setStyleSheet("color:dark");
    ui->label_5->setText("Ход соперника");
    flag = false;
  }

 else if(simbol == 'o'){
      simbol = 'x';
      imgpatch = ":/img/x.png";
      pic.load(imgpatch);
      pic = pic.scaled(ui->tableWidget->width(), ui->tableWidget->height(), Qt::KeepAspectRatio);
      mItem = new QTableWidgetItem();
      mItem->setData(Qt::DecorationRole, pic);
      ui->tableWidget->setItem(0,0, mItem);
      label_6->setStyleSheet("color:dark");
      ui->label_5->setText("Ожидание решения соперника");
      flag = false;

  }
}

void MainWindow::on_pushButton_4_clicked()
{
//кнока продолжить
    ui->label_7->setText("Раунд: "+QString::number(round));
    draw();
    ui->pushButton_4->setStyleSheet("");
    ui->pushButton_4->setEnabled(false);
    socket->write("{\"type\":\"continue\"}");//отправка на сервер о продолжении
}
//===========================================
void MainWindow::on_m_showEmojiListButton_clicked()
{
//кнопка открытия меню смайликов
    if (m_emojiListWidget->isVisible()) {
                m_emojiListWidget->hide();
                ui->textEdit_chatMessage->setFocus();
                QApplication::setActiveWindow(ui->textEdit_chatMessage);
            }
            else {
                m_emojiListWidget->show();
                QPoint point = QPoint(ui->textEdit_chatMessage->width()-m_emojiListWidget->width(), ui->textEdit_chatMessage->height()-1);
                QPoint pos = ui->textEdit_chatMessage->mapToGlobal(point);
                m_emojiListWidget->move(pos);
                QApplication::setActiveWindow(m_emojiListWidget);
                m_emojiListWidget->setFocus();
            }

    // выпадающий список со смайликами
        QDirIterator it(":emoji", { "*.png" });
        QSize iconSize(32,32);
        while (flemoji && it.hasNext()) {
            QString iconPath = it.next();

            QToolButton *emoji = new QToolButton;
            emoji->setIconSize(iconSize);
            emoji->setIcon(QIcon(iconPath));
            emoji->setFixedSize(iconSize);
            emoji->resize(iconSize);
            emoji->setStyleSheet("QToolButton { border: none; padding: 0px; }");
            connect(emoji, &QToolButton::clicked, this, [=](){
                ui->textEdit_chatMessage->insertHtml(QString("<img src=\"%1\" width=\"32\" height=\"32\">").arg(QString(iconPath).replace("22x22", "14x14")));
                emjpatch = iconPath;
                emjsend = true;
            });

            QListWidgetItem *item = new QListWidgetItem;
            item->setSizeHint(emoji->sizeHint());
            m_emojiListWidget->addItem(item);
            m_emojiListWidget->setItemWidget(item, emoji);

        }

        ui->textEdit_chatMessage->installEventFilter(this);
        flemoji = false;
}
//===============================================
void MainWindow::on_button_yes_clicked()
{
    ui->tableWidget_2->clear();
    ui->label_3->clear();
    round  =  0;
    win    =  0;
    loose  =  0;
    radius = 60;

    ui->label_5->setText("Ожидание решения соперника");
    socket->write("{\"type\":\"yes\"}");

    ui->label_continue->setEnabled(false);
    ui->button_no->setEnabled(false);
    ui->button_yes->setEnabled(false);
    ui->label_continue->setText("");
    ui->button_yes->setText("");
    ui->button_no->setText("");

}
//=============================================
void MainWindow::on_button_no_clicked()
{
    ui->tableWidget_2->clear();
    ui->label_3->clear();
    round  =  0;
    win    =  0;
    loose  =  0;
    radius = 60;

    ui->label_5->setText("Ожидание решения соперника");
    socket->write("{\"type\":\"no\"}");

    ui->label_continue->setEnabled(false);
    ui->button_no->setEnabled(false);
    ui->button_yes->setEnabled(false);
    ui->label_continue->setText("");
    ui->button_yes->setText("");
    ui->button_no->setText("");
}
//========================================
QTcpSocket* MainWindow::socket{};
QString MainWindow::imgpatch{};
QString MainWindow::simbol{};
bool MainWindow::flag{false};
QLabel* MainWindow::label_6{};






