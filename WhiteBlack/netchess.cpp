#include "netchess.h"
#include "ui_netchess.h"
#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QMessageBox>
#include <windows.h>
#include <QLCDNumber>
#include <QTime>
QString xy="";
QString msg="";
int ttx=0;
int dir[8][2]={{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
int time=0;
int d[20]={2,2};   //记录前一步棋子数
int WeCout;
QString name;//名字
QLabel *BlackM;//黑色方的消息
QLabel *WhiteM;//白色方的消息
QLabel *BlackP;
QLabel *WhiteP;
QMovie *movie1;//五五开的头像
QMovie *movie2;//卢本伟的头像
QMovie *Mov;
QLabel *Welcome;
QLabel *WhiteTitle;
QLabel *BlackTitle;
QLabel *WhiteEat;
QLabel *BlackEat;

NetChess::NetChess(QWidget *parent,QString uname) :
    QWidget(parent),
    ui(new Ui::NetChess)
{
    ui->setupUi(this);
    //用户
    name = uname;
    //设置窗口图标
    QIcon icon;
    icon.addFile(QStringLiteral(":/new/image/new.ico"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);

    //配置参数
    n = 54;
    flag = 1;//白棋先手
    down=1;
    px = -100;
    py = -100;
    pag=0;
    Tc=0;
    //初始化棋盘4个棋子
    for(int i = 0;i<9;i++)
    {
        for(int j= 0;j<9;j++)
        {
           game[i][j]=-1;
           game1[i][j]=-1;
        }
    }
        game1[4][4]=1;
        game1[5][5]=1;
        game1[4][5]=0;
        game1[5][4]=0;
        index=0;   //控制悔棋数组下标
        index1=0;   //控制最多消除数组下标
        count1=0;
        this->setCursor(Qt::BlankCursor); //隐藏鼠标
        time2=0;   //卢本伟的
        ltime = 60; //时间赋初值
        t = new QTimer(this);
        connect(t,SIGNAL( timeout()),this ,SLOT( mySlot()));

        //设置button图片
        QIcon a(":/new/image/black.png");
        ui->pushButton->setIcon(a);
        ui->pushButton_2->setIcon(a);
        ui->pushButton_repentchess->setIcon(a);
        ui->pushButton_start_again->setIcon(a);
        ui->pushButton_3->setIcon(a);
        ui->GiveUp->setIcon(a);
        ui->pushButton_4->setIcon(a);
        ui->Watch->setIcon(a);

        //设置界面
        QFont font("Microsoft YaHei", 12, 50);
        //黑白棋时间显示
        WhiteLcd = new QLCDNumber(this);
        BlackLcd = new QLCDNumber(this);
        WhiteLcd->setSegmentStyle(QLCDNumber::Flat);
        BlackLcd->setSegmentStyle(QLCDNumber::Flat);
        WhiteLcd->setDigitCount(4);
        BlackLcd->setDigitCount(4);
        WhiteLcd->setGeometry(570,195,80,30);
        BlackLcd->setGeometry(570,420,80,30);

        //实现黑白棋吃子个数
        WhiteNum = new QLabel(this);
        BlackNum = new QLabel(this);
        WhiteNum->setStyleSheet("color:red");
        BlackNum->setStyleSheet("color:red");
        WhiteNum->setText("");
        WhiteNum->setGeometry(600,120,220,80);
        WhiteNum->setFont(font);
        BlackNum->setGeometry(600,350,220,80);
        BlackNum->setText("");
        BlackNum->setFont(font);

        //黑白棋名称
        WhiteTitle = new QLabel(this);
        BlackTitle = new QLabel(this);
        WhiteTitle->setStyleSheet("color:black");
        BlackTitle->setStyleSheet("color:black");
        WhiteTitle->setGeometry(520,170,220,80);
        WhiteTitle->setText("");
        WhiteTitle->setFont(font);
        BlackTitle->setGeometry(520,395,220,80);
        BlackTitle->setText("");
        BlackTitle->setFont(font);

        //吃子个数名称
        WhiteEat = new QLabel(this);
        BlackEat = new QLabel(this);
        WhiteEat->setStyleSheet("color:black");
        BlackEat->setStyleSheet("color:black");
        WhiteEat->setGeometry(520,120,220,80);
        WhiteEat->setText("棋子数");
        WhiteEat->setFont(font);
        BlackEat->setGeometry(520,350,220,80);
        BlackEat->setText("棋子数");
        BlackEat->setFont(font);

        //人物头像
        WhiteP = new QLabel(this);
        BlackP = new QLabel(this);
        WhiteP->setGeometry(530,45,100,100);
        movie1 = new QMovie;
        movie1->setFileName(":/new/image/2345.gif");
        BlackP->setGeometry(530,275,100,100);
        movie2 = new QMovie;
        movie2->setFileName(":/new/image/6789.gif");
        WhiteP->hide();
        BlackP->hide();

        //头像提示信息
        QFont font1("Microsoft YaHei", 15, 50);
        WhiteM = new QLabel(this);
        BlackM = new QLabel(this);
        WhiteM->setGeometry(560,70,500,50);
        WhiteM->setStyleSheet("color:red");
        WhiteM->setText("");
        WhiteM->setFont(font1);
        BlackM->setGeometry(560,300,500,50);
        BlackM->setStyleSheet("color:red");
        BlackM->setText("");
        BlackM->setFont(font1);

        c[0]=2;
        c[1]=2;

        ui->lcdNumber->display(60);
        setMouseTracking(true);
        ui->lcdNumber->display(ltime);
        ui->pushButton_repentchess->setEnabled(false);
        ui->pushButton_start_again->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->Watch->setEnabled(false);

        //网络功能按钮
        ui->Connect->setFlat(true);
        ui->Connect->setIcon(a);
        ui->ipEdit->setText("127.0.0.1");
        ui->portEidt->setText("8080");
        socket = new QTcpSocket;
        isConnect=1;

        //联机按钮连接
        connect(socket,SIGNAL(readyRead()),this,SLOT(readslot()));

        //设置网络聊天
        ui->lineEdit->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->GiveUp->setEnabled(false);

        //设置欢迎界面
        Welcome = new QLabel(this);
        Welcome->setGeometry(155,155,200,200);
        Mov = new QMovie;
        Mov->setFileName(":/new/image/djs01.gif");
        Welcome->setMovie(Mov);
        Welcome->setScaledContents(true);
        Welcome->hide();
        WeCout=5;

        //设置按钮透明化
        ui->pushButton->setFlat(true);
        ui->pushButton_2->setFlat(true);
        ui->pushButton_3->setFlat(true);
        ui->pushButton_4->setFlat(true);
        ui->pushButton_repentchess->setFlat(true);
        ui->pushButton_start_again->setFlat(true);
        ui->GiveUp->setFlat(true);
        ui->Watch->setFlat(true);

        //设置音频播放器
        player = new QMediaPlayer;
        player->setMedia(QUrl::fromLocalFile("0002.mp3"));
        player->setVolume(100);

        //一键喊话信号槽
        connect(ui->comboBox,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(on_box(const QString &)));

        // FramelessWindowHint属性设置窗口去除边框;
        // WindowMinimizeButtonHint 属性设置在窗口最小化时，点击任务栏窗口可以显示出原窗口;
        this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
        // 设置窗口背景透明;
        setAttribute(Qt::WA_TranslucentBackground);
        // 关闭窗口时释放资源;
        setAttribute(Qt::WA_DeleteOnClose);

        m_titleBar = new Interface(this);
        m_titleBar->move(0, 0);

        connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
        connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
        connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
        connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));

        // 设置标题栏跑马灯效果，可以不设置;
        m_titleBar->setTitleRoll();
        m_titleBar->setTitleIcon(":/new/image/black.png");
        QString s1 = "\t\t\t\t\t尊敬的";
        QString s2 = "当前模式：人人对战（联机）";
        QString user_str = QString("%1%2,%3").arg(s1).arg(name).arg(s2);
        m_titleBar->setTitleContent(user_str);
        m_titleBar->setButtonType(MIN_MAX_BUTTON);
        m_titleBar->setTitleWidth(this->width());

}

NetChess::~NetChess()
{
    delete ui;
    delete socket;
    delete WhiteLcd;
    delete BlackLcd;
    delete WhiteNum;
    delete BlackNum;
    delete WhiteM;
    delete BlackM;
    delete WhiteP;
    delete BlackP;
}

void NetChess::fun(int &d, int &b, int f[9][9], int e)
{
    int x,y;
    for(int i = 0;i<8;i++)
    {
        count=0;
        x=d;
        y=b;
        x=x+dir[i][0];
        y=y+dir[i][1];
        if(x>8||y>8||x<1||y<1)
            continue;
        while(f[x][y]!=e&&f[x][y]!=-1)
        {
               x=x+dir[i][0];
               y=y+dir[i][1];
               if(x>8||y>8||x<1||y<1)
                   break;
                count++;
        }
        if(x>8||y>8||x<1||y<1)
        {
             continue;
        }
        else if(f[x][y]==e)
        {
            if(e==0)
            {
                c[0]+=count;
                c[1]-=count;
                BlackNum->setText(QString::number(c[0]));
                WhiteNum->setText(QString::number(c[1]));
            }
            else
            {
                c[1]+=count;
                c[0]-=count;
                WhiteNum->setText(QString::number(c[1]));
                BlackNum->setText(QString::number(c[0]));
            }
            if(c[0]==0||c[1]==0)
            {
                QTextStream out(socket);
                xy = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9").arg(0).arg(0)
                        .arg(flag).arg(c[0])
                        .arg(c[1]).arg(0)
                        .arg(4).arg(str).arg(0);
                out<<xy;
                if(c[0]==0)
                {
                    int mes = QMessageBox::information(this,("提示"),
                                                       ("白棋获胜"));
                    if(mes == QMessageBox::Ok)
                    {
                         t->stop();
                         ui->pushButton_repentchess->setEnabled(false);
                         ui->pushButton_start_again->setEnabled(false);
                         ui->GiveUp->setEnabled(false);
                    }
                }
                else if(c[1] == 0)
                 {
                    QTextStream out(socket);
                    xy = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9").arg(0).arg(0)
                            .arg(flag).arg(c[0])
                            .arg(c[1]).arg(0)
                            .arg(5).arg(str).arg(0);
                     out<<xy;
                     int mes = QMessageBox::information(this,("提示"),
                                                        ("黑棋获胜"));
                     if(mes == QMessageBox::Ok)
                     {
                          t->stop();
                          ui->pushButton_repentchess->setEnabled(false);
                          ui->pushButton_start_again->setEnabled(false);
                          ui->GiveUp->setEnabled(false);
                     }
                 }
                 init();
                 break;
            }
            while(count-->0)
            {
                x = x-dir[i][0];
                y = y-dir[i][1];
                if(x>8||y>8||x<1||y<1)
                {
                     break;
                }
                 f[x][y]=e;
            }
        }
    }

}

void NetChess::fun1(int &d, int &b, int f[9][9], int c)
{
    int x,y;
    for(int i = 0;i<8;i++)
    {
        count=0;
        x=d;
        y=b;
        x=x+dir[i][0];
        y=y+dir[i][1];
        if(x>8||y>8||x<1||y<1)
            continue;
        while(f[x][y]!=c&&f[x][y]!=-1)
        {

               x=x+dir[i][0];
               y=y+dir[i][1];
               if(x>8||y>8||x<1||y<1)
               {
                   count=0;
                   break;
               }
                count++;

        }
        if(count>0)
        {
            if(f[x][y]==c)
            {
                count1=1;
            }
        }

   }
}

void NetChess::internet_chess(QMouseEvent *p)
{
    count1=0;
    int x=(p->x())/n;
    int y=(p->y())/n;
    if(game[y][x]==-1)
    {
        fun1(y,x,game,flag);
        if(count1==1)
        {
            if(down==1)
            {
                for(int i=1;i<9;i++)
                    for(int j=1;j<9;j++)
                        game1[i][j]=game[i][j];
                d[0]=c[0];
                d[1]=c[1];
                down=0;
                this->update();
                int x=(p->x())/n;
                int y=(p->y())/n;
                if((p->x()>=n&&p->x()<=9*n)&&(p->y()>=n&&p->y()<=9*n))
                {
                    if(game[y][x]==-1)
                    {
                        this->update();
                        count1=0;
                        fun1(y,x,game,flag);
                        this->update();
                        if(count1==1)
                        {
                            count1=0;
                            fun(y,x,game,flag);
                            game[y][x]=flag;  //下棋子
                            pag++;
                            winer(!flag);
                            if(flag==1)
                                c[1]+=1;
                            else
                                c[0]+=1;
                            QString s="";
                            ui->pushButton_repentchess->setEnabled(true);
//                            for(int i =0;i<9;i++)
//                                for(int j =0;j<9;j++)
//                                {
//                                    if(i==8&&j==8)
//                                        s+=QString("%0").arg(game[i][j]);
//                                    else
//                                         s+=QString("%0:").arg(game[i][j]);
//                                }
                            QTextStream out(socket);
//                            xy = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14").arg(x).arg(y)
//                                    .arg(flag).arg(c[0])
//                                    .arg(c[1]).arg(0)
//                                    .arg(0).arg(0).arg(0).arg(0).arg(0).arg(0).arg(0).arg(s);
                            xy = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10").arg(x).arg(y)
                                    .arg(flag).arg(c[0])
                                    .arg(c[1]).arg(0)
                                    .arg(0).arg(0).arg(0).arg(0);
                            out<<xy;
                            this->update();
                            ltime = 60;
                            WhiteNum->setText(QString::number(c[1]));
                            BlackNum->setText(QString::number(c[0]));
                        }
                    }
                }
            }
        }
    }

}


void NetChess::init()
{
    WhiteM->setText("");
    WhiteP->setText("");
    BlackM->setText("");
    BlackP->setText("");
    for(int i = 0;i<9;i++)
        for(int j= 0;j<9;j++)
            game[i][j]=-1;
    game[4][4]=-1;
    game[5][5]=-1;
    game[4][5]=-1;
    game[5][4]=-1;  //棋子数组初始化
    ltime = 60;
    WeCout=5;
    c[1]=2;
    c[0]=2;
    flag=1;
    time2=0;
    t->stop();
    WhiteP->hide();
    BlackP->hide();
    movie1->stop();
    movie2->stop();
    WhiteNum->setText("");
    BlackNum->setText("");
    WhiteLcd->display("0:0");
    BlackLcd->display("0:0");
    ui->lcdNumber->display("0");
}

void NetChess::winer(int flag10)
{
    int index2=0;
    index1=0;
    count1=0;
    for(int i=1;i<9;i++)
        for(int j=1;j<9;j++)
        {

            if(game[i][j]==-1)
            {
                index2=1;
                fun1(i,j,game,flag10);
                if(count1==1)
                    continue;
            }
        }
    if(index2==0)
    {
        if(c[0]>c[1])
        {
            QTextStream out(socket);
            xy = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9").arg(0).arg(0)
                    .arg(flag10).arg(c[0])
                    .arg(c[1]).arg(0)
                    .arg(5).arg(str).arg(0);
            out<<xy;
            int mes = QMessageBox::information(this,("提示"),("黑棋获胜"));
            if(mes == QMessageBox::Ok)
            {
                t->stop();
                ui->pushButton_repentchess->setEnabled(false);
                ui->pushButton_start_again->setEnabled(false);
                ui->GiveUp->setEnabled(false);
            }
        }
        else if(c[0]<c[1])
        {
            QTextStream out(socket);
            xy = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9").arg(0).arg(0)
                    .arg(flag10).arg(c[0])
                    .arg(c[1]).arg(0)
                    .arg(4).arg(str).arg(0);
            out<<xy;
            int mes = QMessageBox::information(this,("提示"),("白棋获胜"));
            if(mes == QMessageBox::Ok)
            {
                t->stop();
                ui->pushButton_repentchess->setEnabled(false);
                ui->pushButton_start_again->setEnabled(false);
                ui->GiveUp->setEnabled(false);
            }
        }
        else
        {
            QTextStream out(socket);
            xy = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9").arg(0).arg(0)
                    .arg(flag10).arg(c[0])
                    .arg(c[1]).arg(0)
                    .arg(6).arg(str).arg(0);
            out<<xy;
            int mes = QMessageBox::information(this,("提示"),("平局"));
            if(mes == QMessageBox::Ok)
            {
                t->stop();
                ui->pushButton_repentchess->setEnabled(false);
                ui->pushButton_start_again->setEnabled(false);
                ui->GiveUp->setEnabled(false);
            }
        }
        init();

    }
    else
    {
        if(count1==0)
        {
            if(flag10==1)
            {
                QTextStream out(socket);
                xy = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9").arg(0).arg(0)
                        .arg(flag10).arg(c[0])
                        .arg(c[1]).arg(0)
                        .arg(7).arg(str).arg(0);
                out<<xy;
                int mes = QMessageBox::information(this,("提示"),("白方没地方可下,黑方获胜"));
                if(mes == QMessageBox::Ok)
                {
                    t->stop();
                    ui->pushButton_repentchess->setEnabled(false);
                    ui->pushButton_start_again->setEnabled(false);
                    ui->GiveUp->setEnabled(false);
                }
            }

            else
            {
                QTextStream out(socket);
                xy = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9").arg(0).arg(0)
                        .arg(flag10).arg(c[0])
                        .arg(c[1]).arg(0)
                        .arg(8).arg(str).arg(0);
                out<<xy;
                int mes = QMessageBox::information(this,("提示"),("黑方地方可下,白方获胜"));
                if(mes == QMessageBox::Ok)
                {
                    t->stop();
                    ui->pushButton_repentchess->setEnabled(false);
                    ui->pushButton_start_again->setEnabled(false);
                    ui->GiveUp->setEnabled(false);
                }
            }

            init();
        }
    }
}

//画棋盘
void NetChess::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/new/image/beijing6.jpeg"));
    //画棋盘
    QPen pen;
    pen.setWidth(3);
    pen.setColor(Qt::red);
    p.setPen(pen);
    for(int i=1;i<=9;i++)
    {
         p.drawLine(n,n*i,n*9,n*i);
    }
    for(int i=1;i<=9;i++)
    {
         p.drawLine(n*i,n,n*i,n*9);
    }

    //画棋子
    for(int i = 1;i<9;i++)
    {
        for(int j= 1;j<9;j++)
        {
            if(game[i][j]==0)
            {
                p.drawPixmap(n*j,n*i,n,n,QPixmap(":/new/image/black.png"));
            }
            else if(game[i][j]==1)
            {
                p.drawPixmap(n*j,n*i,n,n,QPixmap(":/new/image/white.png"));
            }
        }
    }

    if(flag==0)
    {
        if((px>=n&&px<=n*9))
        {
            this->setCursor(QCursor(QPixmap(":/new/image/ssblack.bmp")));

            if((py>=n&&py<=n*9))
            {
                this->setCursor(QCursor(QPixmap(":/new/image/ssblack.bmp")));
            }
            else
            {
                this->setCursor(QCursor(QPixmap(":/new/image/777.png")));
            }
        }
        else
        {
            this->setCursor(QCursor(QPixmap(":/new/image/777.png")));
        }
    }
    else
    {   if((px>=n&&px<=n*9))
        {
            this->setCursor(QCursor(QPixmap(":/new/image/sswhite.bmp")));

            if((py>=n&&py<=n*9))
            {
                this->setCursor(QCursor(QPixmap(":/new/image/sswhite.bmp")));
            }
            else
            {
                this->setCursor(QCursor(QPixmap(":/new/image/777.png")));
            }
        }
        else
        {
            this->setCursor(QCursor(QPixmap(":/new/image/777.png")));
        }

    }
    this->update();

}

void NetChess::mouseMoveEvent(QMouseEvent *p)
{
    px = p->x();
    py = p->y();
}

void NetChess::mousePressEvent(QMouseEvent *p)
{
    //if(Tc==1)
   // {
        ui->pushButton_2->setEnabled(true);
        internet_chess(p);
   // }
}

//键盘响应
void NetChess::keyPressEvent(QKeyEvent *)
{
    if(!ui->lineEdit->text().isEmpty())
    {
        QString str = ui->lineEdit->text();
        QTextStream out(socket);
        QString stt = QString("%1:%2").arg(name).arg(str);
        xy = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12").arg(0).arg(0)
                .arg(flag).arg(c[0])
                .arg(c[1]).arg(0)
                .arg(1).arg(stt).arg(0)
                .arg(0).arg(0).arg(str);
        out<<xy;
        ui->listWidget->addItem(stt);
        ui->lineEdit->setText("");
        WhiteM->setText(str);
    }
}

//返回上一级
void NetChess::on_pushButton_clicked()
{
    t->stop();
    this->close();
    emit MySig();
}

//退出
void NetChess::on_pushButton_2_clicked()
{
    int temp = QMessageBox::question(this,"Ques","确定要退出？",
                                     QMessageBox::Yes,QMessageBox::No);
    if(temp==QMessageBox::Yes)
    {
        this->close();
        t->stop();
    }
    else if(temp==QMessageBox::No)
    {
        return;
    }
}

//再来一局
void NetChess::on_pushButton_start_again_clicked()
{
    init();
    t->stop();
}

//悔棋
void NetChess::on_pushButton_repentchess_clicked()
{
    QTextStream out(socket);
    xy = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9").arg(0).arg(0)
            .arg(flag).arg(c[0])
            .arg(c[1]).arg(1).arg(0)
            .arg(0).arg(0);
    out<<xy;
    for(int i=1;i<9;i++)
        for(int j=1;j<9;j++)
            game[i][j]=game1[i][j];
    c[0]=d[0];
    c[1]=d[1];
    down=1;
    WhiteNum->setText(QString::number(c[1]));
    BlackNum->setText(QString::number(c[0]));
    ltime=60;
}

//网络槽函数
void NetChess::readslot()
{
    if(socket->bytesAvailable()>0)
    {
        QTextStream in(socket);
        in>>msg;
        int x1=msg.section(',',0,0).toInt();
        int y1=msg.section(',',1,1).toInt();
        int flag1=(msg.section(',',2,2).toInt());
        int c1 = msg.section(',',3,3).toInt();
        int c2 = msg.section(',',4,4).toInt();
        int h = msg.section(',',5,5).toInt();
        int gg = msg.section(',',6,6).toInt();
        QString tt = msg.section(',',7,7);
        QString mes = msg.section(',',8,8);
        QString user = msg.section(',',9,9);
        int time11 = msg.section(',',10,10).toInt();
        QString sst = msg.section(',',11,11);
        QString chat = msg.section(',',12,12);
//        QString s = msg.section(',',13,13);
//        qDebug()<<"ewqeqweqw"<<s;
       // if(pag==0)
     //   {
            if(gg==1)
            {
                ui->listWidget->addItem(tt);
                BlackM->setText(sst);
            }
            else if(gg==2)
            {
                BlackTitle->setText(user);
                BlackP->setMovie(movie1);
                movie1->start();
                BlackP->setScaledContents(true);
                BlackM->setText(mes);
                BlackP->show();
                if(BlackM->text()==WhiteM->text())
                {
                    WhiteP->setMovie(movie2);
                    movie2->start();
                    WhiteP->setScaledContents(true);
                    Welcome->show();
                    Mov->start();
                    t->start(1000);
                }
            }
            else if(gg==3)
            {
                int mes = QMessageBox::information(this,("提示"),("对面投降,你获胜"));
                if(mes == QMessageBox::Ok)
                {
                    t->stop();
                    ui->pushButton_repentchess->setEnabled(false);
                    ui->pushButton_start_again->setEnabled(false);
                    ui->GiveUp->setEnabled(false);
                }
                init();
            }
            else if(gg==4)
            {
                int mes = QMessageBox::information(this,("提示"),("白棋获胜"));
                if(mes == QMessageBox::Ok)
                {
                    t->stop();
                    ui->pushButton_repentchess->setEnabled(false);
                    ui->pushButton_start_again->setEnabled(false);
                    ui->GiveUp->setEnabled(false);
                }
                init();
            }
            else if(gg==5)
            {
                int mes = QMessageBox::information(this,("提示"),("黑棋获胜"));
                if(mes == QMessageBox::Ok)
                {
                    t->stop();
                    ui->pushButton_repentchess->setEnabled(false);
                    ui->pushButton_start_again->setEnabled(false);
                    ui->GiveUp->setEnabled(false);
                }
                init();
            }
            else if(gg==6)
            {
                int mes = QMessageBox::information(this,("提示"),("平局"));
                if(mes == QMessageBox::Ok)
                {
                    t->stop();
                    ui->pushButton_repentchess->setEnabled(false);
                    ui->pushButton_start_again->setEnabled(false);
                    ui->GiveUp->setEnabled(false);
                }
                init();
            }
            else if(gg==7)
            {
                int mes = QMessageBox::information(this,("提示"),("白方没地方可下,黑方获胜"));
                if(mes == QMessageBox::Ok)
                {
                    t->stop();
                    ui->pushButton_repentchess->setEnabled(false);
                    ui->pushButton_start_again->setEnabled(false);
                    ui->GiveUp->setEnabled(false);
                }
                init();
            }
            else if(gg==8)
            {
                int mes = QMessageBox::information(this,("提示"),("黑方没地方可下,白方获胜"));
                if(mes == QMessageBox::Ok)
                {
                    t->stop();
                    ui->pushButton_repentchess->setEnabled(false);
                    ui->pushButton_start_again->setEnabled(false);
                    ui->GiveUp->setEnabled(false);
                }
                init();
            }
            else if(gg==9)
            {
                str = QString("%1:%2").arg(0).arg(time11%60);
                WhiteLcd->display(str);
            }
            else if(gg==10)
            {
                ui->listWidget->addItem(chat);
                // BlackM->setText(sst);
                player->play();
            }
            else if(gg==0)
            {
                if(h==0)
                {
                    down=1;
                    WhiteNum->setText(QString::number(c2));
                    BlackNum->setText(QString::number(c1));
                    for(int i=1;i<9;i++)
                        for(int j=1;j<9;j++)
                            game1[i][j]=game[i][j];
                    d[0]=c[0];
                    d[1]=c[1];
                    game[y1][x1] = flag1;
                    ltime=60;
                    if(flag1==1)
                        c[1]+=1;
                    else
                        c[0]+=1;
                    fun(y1,x1,game,flag1);
                    //winer();
                    ui->pushButton_repentchess->setEnabled(false);
                    flag=!flag1;
                }
                else
                {
                    down=0;
                    for(int i=1;i<9;i++)
                        for(int j=1;j<9;j++)
                            game[i][j]=game1[i][j];
                    c[0]=d[0];
                    c[1]=d[1];
                    WhiteNum->setText(QString::number(c[1]));
                    BlackNum->setText(QString::number(c[0]));
                    ltime=60;
                }
                this->update();
            }
        //}
        //else if(pag==1)
       // {
           // t->start(1000);
          //  if(gg==9)
           // {
//                if(flag==1)
//                {
//                    str = QString("%1:%2").arg(0).arg(time11%60);
//                    WhiteLcd->display(str);
//                }
//                else if(flag==0)
//                {
//                    str = QString("%1:%2").arg(0).arg(time11%60);
//                    BlackLcd->display(str);
//         //       }
           // }

           // int nw=0;
          //  for(int i=0;i<9;i++)
              //  for(int j=0;j<9;j++)
              //  {
                  //  game[i][j]=s.section(":",nw,nw).toInt();
                  //  nw++;
               // }

       // }
    }

}

//时钟信号槽
void NetChess::mySlot()
{
    ltime--;
    WeCout--;
    if(WeCout==0)
    {
        game[4][4]=1;
        game[5][5]=1;
        game[4][5]=0;
        game[5][4]=0;  //棋子数组初始化
        Welcome->hide();
        Mov->stop();
        BlackNum->setText("2");
        WhiteNum->setText("2");
    }
    if(WeCout<0)
    {
        BlackM->setText("");
        WhiteM->setText("");
    }
    if(Tc==0)
    {
        BlackP->setMovie(movie2);
        movie2->start();
        BlackP->setScaledContents(true);
        WhiteP->setMovie(movie1);
        movie1->start();
        WhiteP->setScaledContents(true);
    }
    if(down==0)
    {
        QString s="";
        for(int i =0;i<9;i++)
            for(int j =0;j<9;j++)
            {
                if(i==8&&j==8)
                    s+=QString("%0").arg(game[i][j]);
                else
                     s+=QString("%0:").arg(game[i][j]);
            }
        time2++;
        QTextStream out(socket);
        xy = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13").arg(0).arg(0)
                .arg(flag).arg(c[0])
                .arg(c[1]).arg(0)
                .arg(9).arg(str).arg(0).arg(0).arg(time2).arg(0).arg(0);
        out<<xy;
        int ti = time2/60;
        str = QString("%1:%2").arg(ti).arg(time2-60*ti);
        BlackLcd->display(str);
    }
    if(ltime==0)
    {
        if(down==1)
        {
            if(flag==1)
            {
                int mes = QMessageBox::information(this,("提示"),("白棋弃权,黑棋获胜"));
                if(mes == QMessageBox::Ok)
                {
                    t->stop();
                    ui->pushButton_repentchess->setEnabled(false);
                    ui->pushButton_start_again->setEnabled(false);
                }
            }
            else if(flag==0)
            {
                int mes = QMessageBox::information(this,("提示"),("黑棋弃权,白棋获胜"));
                if(mes == QMessageBox::Ok)
                {
                    t->stop();
                    ui->pushButton_repentchess->setEnabled(false);
                    ui->pushButton_start_again->setEnabled(false);
                }
            }
        }
        else if(down==0)
        {
            if(flag==0)
            {
                int mes = QMessageBox::information(this,("提示"),("白棋弃权,黑棋获胜"));
                if(mes == QMessageBox::Ok)
                {
                    t->stop();
                    ui->pushButton_repentchess->setEnabled(false);
                    ui->pushButton_start_again->setEnabled(false);
                }
            }
            else if(flag==1)
            {
                int mes = QMessageBox::information(this,("提示"),("黑棋弃权,白棋获胜"));
                if(mes == QMessageBox::Ok)
                {
                    t->stop();
                    ui->pushButton_repentchess->setEnabled(false);
                    ui->pushButton_start_again->setEnabled(false);
                }
            }
        }
        init();
    }
    ui->lcdNumber->display(ltime);
}

//联机
void NetChess::on_Connect_clicked()
{
    if(isConnect)
    {
        if(ui->ipEdit->text().isEmpty()||ui->portEidt->text().isEmpty())
        {
            QMessageBox::critical(this,NULL,"ip or port is NULL");
            return;
        }
        QString str = ui->ipEdit->text();
        quint16 p = ui->portEidt->text().toShort();
        socket->connectToHost(str,p);
        ui->Connect->setText("断开");
        ui->lineEdit->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->ipEdit->setEnabled(false);
        ui->portEidt->setEnabled(false);
        ui->pushButton_3->setEnabled(true);
        ui->Watch->setEnabled(true);
        connect(socket,SIGNAL(readyRead()),this,SLOT(readslot()));
        isConnect=0;
    }
    else
    {
        ui->Connect->setText("联机");
        ui->lineEdit->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->ipEdit->setEnabled(true);
        ui->portEidt->setEnabled(true);
        ui->pushButton_3->setEnabled(false);
        ui->Watch->setEnabled(false);
        socket->disconnectFromHost();
        isConnect=1;
    }
}

//开始游戏
void NetChess::on_pushButton_3_clicked()
{
    QString str = "准备!";
    WhiteP->setMovie(movie1);
    movie1->start();
    WhiteP->setScaledContents(true);
    WhiteTitle->setText(name);
    WhiteM->setText("准备!");
    if((WhiteM->text())==(BlackM->text()))
    {
        BlackP->setMovie(movie2);
        movie2->start();
        BlackP->setScaledContents(true);
        Welcome->show();
        Mov->start();
        t->start(1000);
    }
    WhiteP->show();
    QTextStream out(socket);
    xy = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13").arg(0).arg(0).arg(flag)
            .arg(c[0]).arg(c[1])
            .arg(0).arg(2).arg(0).arg(str)
            .arg(name).arg(0).arg(0).arg(0);
    out<<xy;
    ui->pushButton_repentchess->setEnabled(true);
    ui->pushButton_start_again->setEnabled(true);
    ui->GiveUp->setEnabled(true);
    ui->Watch->setEnabled(false);
}

//发送消息
void NetChess::on_pushButton_4_clicked()
{
    if(!ui->lineEdit->text().isEmpty())
    {
        QString str = ui->lineEdit->text();
        QString stt = QString("%1:%2").arg(name).arg(str);
        QTextStream out(socket);
        xy = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12").arg(0).arg(0)
                .arg(flag).arg(c[0])
                .arg(c[1]).arg(0)
                .arg(1).arg(stt).arg(0)
                .arg(0).arg(0).arg(str);
        out<<xy;
        ui->listWidget->addItem(stt);
        ui->lineEdit->setText("");
        WhiteM->setText(str);
    }
}

//投降
void NetChess::on_GiveUp_clicked()
{
    int temp = QMessageBox::question(this,"Ques","确定要投降？",
                                     QMessageBox::Yes,QMessageBox::No);
    if(temp==QMessageBox::Yes)
    {
        int mes = QMessageBox::information(this,("提示"),("你已投降"));
        if(mes == QMessageBox::Ok)
        {
             t->stop();
             ui->pushButton_repentchess->setEnabled(false);
             ui->pushButton_start_again->setEnabled(false);
             ui->GiveUp->setEnabled(false);
             QTextStream out(socket);
             xy = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9").arg(0).arg(0)
                     .arg(flag).arg(c[0])
                     .arg(c[1]).arg(0)
                     .arg(3).arg(str).arg(0);
             out<<xy;
        }
        init();
    }
    else if(temp==QMessageBox::No)
    {
        return;
    }
}

//一键喊话
void NetChess::on_box(const QString &text)
{
    if(text == "快点吧我等的花儿都谢了")
    {
        player->play();
        QString stt = QString("%1:%2").arg(name).arg(text);
        ui->listWidget->addItem(text);
       // WhiteM->setText(text);
        QTextStream out(socket);
        xy = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13").arg(0).arg(0)
                .arg(flag).arg(c[0])
                .arg(c[1]).arg(0)
                .arg(10).arg(0).arg(0)
                .arg(0).arg(0).arg(text).arg(stt);
        out<<xy;
    }
}

//观战
void NetChess::on_Watch_clicked()
{
    pag=1;
//    if(Tc==0)
//    {
//        pag=1;
//        Tc=1;
//        ui->Watch->setText("取消");
//        QTextStream out(socket);
//        xy = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14").arg(0).arg(0)
//                .arg(flag).arg(c[0])
//                .arg(c[1]).arg(0)
//                .arg(10).arg(0).arg(0)
//                .arg(0).arg(0).arg(0).arg(0).arg(pag);
//        out<<xy;
//    }
    if(Tc==0)
    {


//    else if(Tc==1)
//    {
//        pag=0;
//        Tc=0;
//        ui->Watch->setText("联机");
   // }
        t->start(1000);
        Tc=1;
        ui->Watch->setText("取消");
        //隐藏所有按钮
        ui->GiveUp->setEnabled(false);
        ui->pushButton->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_repentchess->setEnabled(false);
        ui->pushButton_start_again->setEnabled(false);
        ui->lineEdit->setEnabled(false);
    }

   else if(Tc==1)
   {
       int mes = QMessageBox::information(this,("提示"),("你已退出观战!"));
       if(mes == QMessageBox::Ok)
       {
           init();
           t->stop();
           Tc=0;
           ui->Watch->setText("观战");
       }
   }
}

//窗口上方按钮响应
void NetChess::onButtonMinClicked()
{
    showMinimized();
}

void NetChess::onButtonRestoreClicked()
{
    QPoint windowPos;
    QSize windowSize;
    m_titleBar->getRestoreInfo(windowPos, windowSize);
    this->setGeometry(QRect(windowPos, windowSize));
}

void NetChess::onButtonMaxClicked()
{
    m_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
    QRect desktopRect = QApplication::desktop()->availableGeometry();
    QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
    setGeometry(FactRect);
}

void NetChess::onButtonCloseClicked()
{
    int temp = QMessageBox::question(this,"Ques","确定要退出？",QMessageBox::Yes,QMessageBox::No);
    if(temp==QMessageBox::Yes)
    {
        t->stop();
        this->close();
    }
    else if(temp==QMessageBox::No)
    {
        return;
    }
}


