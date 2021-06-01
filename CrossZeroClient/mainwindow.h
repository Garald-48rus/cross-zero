#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QStandardItem>
#include <QJsonArray>
#include <QString>
#include <QBuffer>
#include <QTableWidgetItem>


#include "start.h"
#include "ui_start.h"
#include "gamefield.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void _sendData();

    void sockReady();
    void sockDisc();


    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

    void on_m_showEmojiListButton_clicked();

    void on_button_yes_clicked();

    void on_button_no_clicked();

private:
    Ui::MainWindow *ui;

private:
    Start* mStart;
    QTableWidgetItem* mItem;
    QGraphicsBlurEffect* Effect;
    QByteArray Data;
    QJsonDocument doc;
    QJsonParseError docError;
    QString name{};
    QPixmap pic;
    QPixmap background;
    qreal radius{60};
    QListWidget* m_emojiListWidget;

    int win{0};
    int loose{0};
    int round{1};
    void draw();

    bool flemoji;
    QString emjpatch{};
    bool emjsend{false};

public:
     static QString imgpatch;
     static QTcpSocket* socket;
     static QString simbol;
     static bool flag;
     static QLabel* label_6;

     static void cmotion();

};
#endif // MAINWINDOW_H
