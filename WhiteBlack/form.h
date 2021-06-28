#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QPaintEvent>
#include <QPainter>
#include <QPoint>
#include <QDebug>
#include <stdlib.h>
#include <QMessageBox>
#include <QString>
#include <QCursor>
#include <QPixmap>
#include <QMovie>
#include <QPalette>
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <windows.h>
#include <QDesktopWidget>
#include <QApplication>
#include <QFile>
#include "chessboard.h"
#include "mediaplayer.h"
#include "sql.h"
#include "interface.h"

class ChessBoard;
namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
    void get_chess_num();
    static int deposit_flag; //0.白子托管,1.黑子托管
    static int movie_flag; //1.播放,0不播放
    static int background_flag; //背景图片显示标志位
    static int song_flag; //0.播放,1.暂停
    static int button_flag; //1.暂停,0.播放

private:
    Ui::Form *ui;
    ChessBoard *w; //棋盘
    MediaPlayer *In; //视频介绍
    Sql *sql; //数据库
    Interface *m_titleBar; //界面美化
    QTimer *timer; //定时器
    //视频、音频播放器
    QMediaPlayer *player;
    QMediaPlayer *play1;
    QMediaPlayer *play2;
    QMediaPlayer *play4;
    int t;   //时间
    char *background_name; //背景图片名
    //菜单栏
    QMenu *menu[10];
    QAction *act[20];
    QMenuBar *menuBar;
    //QStatusBar *status;

signals:
    void mySignal();

private slots:
    void on_start_clicked(); //开始
    void on_pause_clicked(); //暂停
    void on_repentchess_clicked(); //悔棋
    void on_skip_clicked();   //跳过
    void on_start_again_clicked();  //重来一局
    void on_deposit_clicked(); //托管
    void mySlot();  //timer信号槽
    void time_return(); //ChessBoard信号槽
    void trigerMenu(QAction *act);//菜单栏信号槽
    //菜单栏
    void onButtonMinClicked();
    void onButtonRestoreClicked();
    void onButtonMaxClicked();
    void onButtonCloseClicked();

private:
    void paintEvent(QPaintEvent *); //绘图事件

};

#endif // FORM_H



