#ifndef START_H
#define START_H

#include <QWidget>

#include <QFileDialog>

namespace Ui {
class Start;
}

class Start : public QWidget
{
    Q_OBJECT

public:
    explicit Start(QWidget *parent = nullptr);
    ~Start();

private:
    Ui::Start *ui;

public:
    QString imgadr{};
    QPixmap* pic;

private slots:
    void on_toolButton_clicked();



    void on_pushButton_clicked();

signals:

    void _addData();

};

#endif // START_H
