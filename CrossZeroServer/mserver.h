#ifndef MSERVER_H
#define MSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QString>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include <QFile>
#include <QDataStream>

class mServer: public QTcpServer
{
    Q_OBJECT
public:
     mServer();

public slots:
    void incomingConnection(qintptr handle) override;
    void slotServerRead();
    void slotClientDisconnected();

private:
    QVector <QTcpSocket *> mTcpSocket;
    QByteArray mData;
    QTcpSocket* mSocket;

    QJsonDocument doc;
    QJsonParseError docError;

    bool flag{false};
    int counter{0};
    int index{0};

    QVector <QString> O;
    QVector <QString> X;
private:
    QVector <QVector<QString>> win{{"0,0", "0,1", "0,2"},{"1,0", "1,1", "1,2"},{"2,0", "2,1", "2,2"},{"0,0", "1,0", "2,0"},
                                  {"0,1", "1,1", "2,1"},{"0,2", "1,2", "2,2"},{"0,0", "1,1", "2,2"},{"0,2", "1,1", "2,0"}};


private:
    bool game(QChar ,QTcpSocket*, QVector<QString> &);

};

#endif // MSERVER_H
