#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <form.h>
#include <QTimer>
#include <QTime>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPixmap>
#include <QCursor>
#include <QApplication>
#include <QPen>
#include <QDebug>
#include <QString>
#include <QPainter>
#include <QMediaPlayer>

class Form;
extern int page;
namespace Ui {
class ChessBoard;
}

class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoard(QWidget *parent = 0);
    ~ChessBoard();
    void Getchess(); //得到棋子的坐标
    void show_mid_fourchess(); //显示初始化棋盘中四个子
    enum GridState{Black,White,Empty}; //枚举（loop的标志位）
    //判断函数：是否吃子
    int judgeRule(int x,int y,int gridNum,void *chess,GridState currentRole);
    GridState ccurrentRole;
    void people_pk_machine(GridState ccurrentRole); //人机对战实现
    void jude_chess_ptop(); //人人对战
    void jude_chess_ptom(); //人机对战
    void ptom_show_fun(); //人机对战显示函数
    void voluation_three_array(); //三维数组赋值
    void begin_chess();  //开始下棋
    void black_deposit(); //黑棋托管
    void show_black_deposit(); //显示黑棋托管
    int count_black; //落子后棋盘上黑子的个数
    int count_white;  //白子的个数
    int chess[8][8];  //记录棋盘上黑白子的坐标
    int chess_regret[64][8][8]; //记录每次落子黑白子的情况，用于悔棋

private:
    Ui::ChessBoard *ui;
    int bw;  //棋盘的总宽度
    int bh;  //棋盘的总高度
    int x;   //每个棋格的宽度
    int y;   //每个棋格的高度
    int px;  //鼠标点击得到的横坐标
    int py;   //鼠标点击得到的纵坐标
    int mid_x; //放置棋子的格子的横坐标
    int mid_y;  //放置棋子的格子的纵坐标
    int startX;
    QPoint point;
    QTimer *ttime; //人机反应时间
    QMediaPlayer *play; //点击棋盘的声音
    QMediaPlayer *player1; //五杀
    QMediaPlayer *player2; //吃子5颗以上
    QMediaPlayer *player3; //四杀
    QMediaPlayer *player4; //三杀
    QMediaPlayer *player5; //双杀

    int chess_two[8][8]; //判断数组(用于人机对战中)
    //二维数组的参数
    static int a;
    static int b;

public:
     static int flag; //对战：1.人人对战,2.人机对战
     static int gg; //人机难度等级 1.入门 2.简单 3.困难
     static int loop; //黑白棋：0.黑棋,2.白棋,3.NULL
     static int black_eat_num; //黑子吃子数目
     static int white_eat_num; //白子吃子数目
     static int c;  //二维数组定点坐标对应的棋盘格数
     static int row;//用格子数转换成顶点坐标是的横坐标
     static int col; //...纵坐标
     int change;  //人机对战中白棋的位置
     static int color_flag;
     static int down_chess; //是否可以下棋

private slots:
     void paintEvent(QPaintEvent *); //绘图
     void mousePressEvent(QMouseEvent *); //鼠标点击
     void TTimer();

signals:
     void control_clock();

};

#endif // CHESSBOARD_H
