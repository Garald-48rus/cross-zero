#include "mserver.h"
#include <QCoreApplication>

mServer::mServer()
{

    if(this->listen(QHostAddress::Any, 5555)){
        qDebug() << "Сервер запущен слушаю";
    }
    else {
        qDebug() << "Сервер не запущен";
    }

}
//===========================================================
void mServer::incomingConnection(qintptr handle)
{
    if(mTcpSocket.size() < 2){

    mSocket = new QTcpSocket(this);
    mSocket->setSocketDescriptor(handle);

     connect(mSocket, &QTcpSocket::readyRead, this, &mServer::slotServerRead);
     connect(mSocket, &QTcpSocket::disconnected, this, &mServer::slotClientDisconnected);
     mTcpSocket.append(mSocket);

     if(mTcpSocket.size() == 2) {
     for(auto &it: mTcpSocket){
      qDebug() << handle << "Client Connect";
      it->write("{\"type\":\"connected\",\"status\":\"yes\"}");
      qDebug() << "Сообщение отправлено";
     }

   }

  }

}
//============================================================================
void mServer::slotServerRead()
{
    for(auto &it : mTcpSocket){

      it->waitForReadyRead(500);
      mData = it->readAll();

    doc = QJsonDocument::fromJson(mData, &docError);

    if (docError.errorString()=="no error occurred")
    {
      //отпрака клиету инфо о подключении

     if (doc.object().value("type").toString() == "info"){

     for(auto &it1 : mTcpSocket){if(it1 != it) {it1->write(mData);}}

   }
//================================================================================
     else if (doc.object().value("type").toString() == "ready") {
         //отправка клиентам символов для игры
           qDebug() << flag;
         if(flag == false){
             it->write("{\"type\":\"symbol\",\"value\":\"o\"}");
             }
         else {
             it->write("{\"type\":\"symbol\",\"value\":\"x\"}");
         }
           flag = true;
     }
//======================================================================
     else if (doc.object().value("type").toString() == "x") {
         //процесс игры для клиента х
         flag = false;

         index = 0;
         X.push_back(doc.object().value("cels").toString());

         if(X.size() >= 3){
          for(auto &itr: win){
              index++;
              if(game('X',it, itr)){ flag = true; break;}
            }
            }

         if(!flag)
         {
         for(auto &it1 : mTcpSocket){if(it1 != it) {it1->write(mData); break;}}
         }

      }
//===========================================================================
        else if (doc.object().value("type").toString() == "o") {
         //процесс игры для клиента о
         flag = false;

         O.push_back(doc.object().value("cels").toString());

         if(O.size() >= 3){             
          for(auto &itr: win){ if(game('O',it, itr)){ flag = true; break;}}
            }

         if(!flag){
         for(auto &it1 : mTcpSocket){if(it1 != it) {it1->write(mData);break;}}
         }


     }
//=================================================================
     else if (doc.object().value("type").toString() == "continue") {

         //отпрака игрокам о продолжении игры новый раунд
         counter ++;

         while (counter != 2) {

             slotServerRead();
         }

        for(auto &it1 : mTcpSocket){it1->write(mData);}

        counter = 0;
     }
//==========================================================================
     else if (doc.object().value("type").toString() == "chat") {
         //чат в игре
       for(auto &it1 : mTcpSocket){if(it1 != it) {it1->write(mData);break;}}
     }
//===============================================================
     else if (doc.object().value("type").toString() == "yes") {

        counter++;
        flag = true;
        while (counter < 2 && flag) {
            slotServerRead();
        }

        if(counter == 2){
         for(auto &it1 : mTcpSocket){it1->write(mData);}
         flag = false;
        }
     }

     else if (doc.object().value("type").toString() == "no") {

         flag = false;
         for(auto &it1 : mTcpSocket) if(it1 != it){it1->write(mData);break;}
     }
  }
//===========================================================================
    else if(docError.errorString()!="no error occurred" && !mData.isEmpty()){
    //отправка игрокам фото
        for(auto &it1 : mTcpSocket)
        {

          if(it1 != it) {it1->write(mData); it1->waitForBytesWritten(500);}

        }

    }

  }
}

//=========================================================================
void mServer::slotClientDisconnected()
{
  mSocket->deleteLater();
 }
//=================================================================
bool mServer::game(QChar ch ,QTcpSocket*  socket, QVector<QString> & win)
{
    //проверка статуса игры выграл, проиграл, ничья
    int counter{0};
    QString str{};

    if(ch == 'X'){

      for(auto &it: X){
       for(auto &it1: win) {
           if(it == it1) {str += it1 + '.'; ++counter;}
      }
       if(counter == 3)  break;
       }

    if(counter == 3){

      socket->write("{\"type\":\"win\",\"cels\":" "\"" +str.toUtf8()+ "\"""}");

      for(auto &it: mTcpSocket){
         if(it != socket){

             it->write("{\"type\":\"loser\",\"cels\":" "\"" +str.toUtf8()+ "\"""}");
             break;
         }
      }

      X.clear();
      O.clear();
      return true;
    }
 }

    if(ch == 'O'){

      for(auto &it: O){
       for(auto &it1: win) {
           if(it == it1)

           {str += it1 + '.';  ++counter;}
       }
       if(counter == 3)  break;

       }
    if(counter == 3){

      socket->write("{\"type\":\"win\",\"cels\":" "\"" +str.toUtf8()+ "\"""}");

      for(auto &it: mTcpSocket){
         if(it != socket){

             it->write("{\"type\":\"loser\",\"cels\":" "\"" +str.toUtf8()+ "\"""}");
             break;
         }

      }

      X.clear();
      O.clear();
      return true;
    }
 }
    if(X.size() == 5 && O.size() == 4 && index == 8){

        X.clear();
        O.clear();
        for(auto &it: mTcpSocket){

               it->write("{\"type\":\"draw\"}");
    }
         return true;
}

    return false;
}
//==================================================================


