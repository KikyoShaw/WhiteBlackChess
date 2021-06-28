#ifndef TCP_H
#define TCP_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
namespace Ui {
class tcp;
}

class tcp : public QWidget
{
    Q_OBJECT

public:
    explicit tcp(QWidget *parent = 0);
    ~tcp();
    QTcpServer *server;
    QTcpSocket *socket;
    QList<QTcpSocket *> list;
private slots:
    void myslot();
    void myslot1();
    void myslot2();
private:
    Ui::tcp *ui;
    int index;
    int player;
};

#endif // TCP_H
