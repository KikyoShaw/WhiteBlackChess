#include "tcp.h"
#include "ui_tcp.h"
#include <QDebug>
tcp::tcp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tcp)
{
    ui->setupUi(this);
    server = new QTcpServer;
    server->listen(QHostAddress::Any,8080);
    connect(server,SIGNAL(newConnection()),this,SLOT(myslot()));
    index=0;
    player=0;
}

tcp::~tcp()
{
    delete ui;
}
void tcp::myslot()
{
    qDebug()<<"有yuanren。。。。";
    if(server->hasPendingConnections())
    {
        player++;
        socket = server->nextPendingConnection();
        connect(socket,SIGNAL(readyRead()),this,SLOT(myslot1()));
        connect(socket,SIGNAL(disconnected()),this,SLOT(myslot2()));
        list<<socket;
        ui->list1->addItem(socket->peerAddress().toString()+":"+QString::number(socket->peerPort()));
        ui->list2->addItem(socket->peerAddress().toString()+":"+QString::number(socket->peerPort())+"连上了");
    }
}

void tcp::myslot1()
{ 
    if(player==2)
    {
        if(list[0]->bytesAvailable()>0)
        {
            QByteArray b = list[0]->readAll();
            list[1]->write(b);
            index=1;
        }
        else if(list[1]->bytesAvailable()>0)
        {
            QByteArray b = list[1]->readAll();
            list[0]->write(b);
            index=0;
        }
    }
    if(player>=3)
    {
        if(list[0]->bytesAvailable()>0)
        {
            QByteArray b = list[0]->readAll();
            list[1]->write(b);
            list[2]->write(b);
            index=1;
        }
        else if(list[1]->bytesAvailable()>0)
        {
            QByteArray b = list[1]->readAll();
            list[0]->write(b);
            list[2]->write(b);
            index=0;
        }
    }
}

void tcp::myslot2()
{
    for(int i=0;i<list.count();i++)
    {
        QTcpSocket *t = list[i];
        if(t->socketDescriptor()==-1)
        {
            player--;
            qDebug()<<t->peerAddress()<<"断开";
            ui->list2->addItem(t->peerAddress().toString()+":"+QString::number(t->peerPort())+"断开了");
            list.removeAt(i);
            delete ui->list1->takeItem(i);
        }
    }
}
