#include "widget.h"
#include "ui_widget.h"
#include "windows.h"
#include <QPalette>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    //设置窗口图标
    QIcon icon;
    icon.addFile(QStringLiteral(":/new/image/new.ico"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    //初始化窗口Form
//    m = new Form();
//    connect(m,SIGNAL(mySignal()),this,SLOT(show()));
//    connect(m,SIGNAL(mySignal()),this,SLOT(my_player()));
    //动态图片加载
    movie = new QMovie();
    movie->setFileName(":/new/image/14830531_62.gif");
    movie->start();
    ui->label_movie->setMovie(movie);
    ui->label_movie->setScaledContents(true);
    this->setGeometry(50,50,500,700);
    //button设计
    QIcon a(":/new/image/white.png");
    ui->playGame->setIcon(a);
    ui->Closegame->setIcon(a);
    ui->Closehelp->setIcon(a);
    ui->Help->setIcon(a);
    ui->playGame->setFlat(true);
    ui->Closegame->setFlat(true);
    ui->Closehelp->setFlat(true);
    ui->Help->setFlat(true);
    //隐藏滑动条
    ui->labelComple_2->hide();
    ui->progressBar_2->hide();

    QFont at;
    at.setPointSize(10);
    ui->labelComple_2->setFont(at);

    movie1 = new QMovie();
    movie1->setFileName(":/new/image/1458.gif");
    movie1->start();
    ui->label->setMovie(movie1);
    ui->label->setScaledContents(true);

    //登录界面的背景音乐
    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("./star.mp3"));
    player->setVolume(60);

    //点击进入游戏后的欢迎词
    player1 = new QMediaPlayer;
    player1->setMedia(QUrl::fromLocalFile("./8431.mp3"));
    player1->setVolume(60);

   //设置帮助
    help_label = new QLabel(this);
    help_label->resize(this->width(),this->height());
    QPixmap map(":/new/image/112.png");
    help_label->setPixmap(map);
    help_label->setScaledContents(true);
    help_label->hide();

}

Widget::~Widget()
{
    delete ui;
    delete movie;
    delete movie1;
    delete movie2;
    delete player;
    delete player1;
}

//设置窗口背景图片
void Widget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/new/image/114.jpg"));
    this->setCursor(QCursor(QPixmap(":/new/image/777.png")));
}

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)//鼠标左击才有效果
         move(e->globalPos()-point);//移动窗口
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    //鼠标事件中包含“按住的是左键”
    if(e->button()==Qt::LeftButton)
    {
        //获取移动的位移量
        point = e->globalPos()-frameGeometry().topLeft();
    }
}

//点击进入游戏按钮
void Widget::on_playGame_clicked()
{
    player1->play();
    //显示滑动条和正在加载...
    ui->progressBar_2->show();
    ui->labelComple_2->show();
    //设置滑动条的进度
    for(int i=0;i<=100;i++)
    {
        ui->progressBar_2->setValue(i);
        Sleep(20);
        if(ui->progressBar_2->value()==100)
        {
           ui->progressBar_2->hide();
           ui->labelComple_2->hide();
           Sleep(50);
           break;
        }
    }
    m = new Form();
    connect(m,SIGNAL(mySignal()),this,SLOT(show()));
    connect(m,SIGNAL(mySignal()),this,SLOT(my_player()));
    //进入Form窗口,关闭当前窗口
    m->show();
    help_label->hide();
    player->setVolume(0);
    player->stop();
    player1->stop();
    this->close();
    qDebug()<<"111";
}

//帮助按钮
void Widget::on_Help_clicked()
{
    ui->label_movie->hide();
    help_label->show();
}

//退出按钮
void Widget::on_Closegame_clicked()
{
    int temp = QMessageBox::question(this,"Ques","确定要退出？",QMessageBox::Yes,QMessageBox::No);
    if(temp==QMessageBox::Yes)
    {
        player->setVolume(0);
        player->stop();
        this->close();
    }
    else if(temp==QMessageBox::No)
    {
        return;
    }
}

//关闭帮助按钮
void Widget::on_Closehelp_clicked()
{
    help_label->hide();
    ui->label_movie->show();
}

void Widget::my_player()
{
    player->setVolume(60);
    player->play();
}


