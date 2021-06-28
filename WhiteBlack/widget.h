#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMovie>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QPoint>
#include <QMediaPlayer>
#include "form.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    int count;
    QPoint point;
    QMediaPlayer *player;

private:
    Ui::Widget *ui;
    QMovie *movie;
    QMovie *movie1;
    QMovie *movie2;
    QMediaPlayer *player1;
    Form *m;
    QLabel *help_label;

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

private slots:
    void on_playGame_clicked(); //进入游戏
    void on_Help_clicked(); //帮助
    void on_Closegame_clicked(); //退出
    void on_Closehelp_clicked(); //关闭帮助
    void my_player();

};

#endif // WIDGET_H
