#ifndef NETCHESS_H
#define NETCHESS_H

#include <QMouseEvent>
#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QLCDNumber>
#include <QStringList>
#include <QString>
#include "pthread.h"
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QCursor>
#include <QPalette>
#include <QKeyEvent>
#include <QMovie>
#include <QMediaPlayer>
#include <QDesktopWidget>
#include <QApplication>
#include <QFile>
#include "interface.h"

namespace Ui {
class NetChess;
}

class NetChess : public QWidget
{
    Q_OBJECT

public:
    explicit NetChess(QWidget *parent = 0,QString uname="");
    ~NetChess();
    QMediaPlayer *player;
    int n;
    int game[9][9];
    int game1[9][9];
    int flag;    //哪方下棋
    int px;
    int py;      //鼠标点击的坐标
    void fun(int &d,int &b,int f[9][9],int e);  //算法函数
    void fun1(int &d,int &b,int f[9][9],int c); //判断
    void internet_chess(QMouseEvent *p);//实现网络人人对战
    void init();  //初始化
    void winer(int flag10);
    int c[2];  //记录当前棋子数
    int count;   //中间棋子数
    int ltime;
    int hq[9][9];
    int index;   //控制悔棋数组下标
    int index1;   //控制最多消除数组下标
    int count1; //是否可以下子变量
    int down; //能否下棋
    int Tc; //观战
    int pag;
    QTimer *t;
    //吃子个数
    QLabel *WhiteNum;
    QLabel *BlackNum;
    int time2;   //黑棋
    QLCDNumber *WhiteLcd;
    QLCDNumber *BlackLcd;
    QString str;
    int model;
    //联机
    QTcpSocket *socket;
    int isConnect;

signals:
    void MySig(); //返回上一级的信号

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

private slots:
    void on_pushButton_clicked(); //返回上一级
    void on_pushButton_2_clicked(); //退出
    void on_pushButton_start_again_clicked(); //再来一局
    void on_pushButton_repentchess_clicked(); //悔棋
    void readslot();
    void mySlot();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_GiveUp_clicked();
    void on_box(const QString &);//一键喊话响应
    void on_Watch_clicked();    
    //菜单栏
    void onButtonMinClicked();
    void onButtonRestoreClicked();
    void onButtonMaxClicked();
    void onButtonCloseClicked();

    void on_Connect_clicked();

private:
    Ui::NetChess *ui;
    Interface *m_titleBar; //界面美化

};

#endif // NETCHESS_H
