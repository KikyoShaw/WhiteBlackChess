#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    qDebug()<<"222";

    this->setGeometry(200,200,900,670);

    //设置鼠标
    this->setCursor(Qt::BlankCursor); //隐藏鼠标

    //添加菜单栏
    //  status = new QStatusBar(this);
    menu[0] = new QMenu("人机模式(A)");
    menu[1] = new QMenu("人人模式(B)");
    menu[2] = new QMenu("关于(G)");
    menu[3] = new QMenu("背景(P)");

    act[0] = new QAction(QIcon(":/new/image/white.png"),tr("单机"),this);
    act[1] = new QAction(QIcon(":/new/image/white.png"),tr("联机"),this);
    menu[1]->addAction(act[0]);
    menu[1]->addAction(act[1]);

    act[2] = new QAction(QIcon(":/new/image/black.png"),tr("入门"),this);
    act[3] = new QAction(QIcon(":/new/image/black.png"),tr("简单"),this);
    act[4] = new QAction(QIcon(":/new/image/black.png"),tr("困难"),this);
    menu[0]->addAction(act[2]);
    menu[0]->addAction(act[3]);
    menu[0]->addAction(act[4]);

    act[6] = new QAction(QIcon(":/new/image/fileopen.png"),tr("玩法介绍"),this);
    act[5] = new QAction(QIcon(":/new/image/helpabout.png"),tr("版本号"),this);
    act[10] = new QAction(QIcon(":/new/image/fallback.png"),tr("返回上级"),this);
    act[9] = new QAction(QIcon(":/new/image/fail.png"),tr("退出"),this);

    menu[2]->addAction(act[6]);
    menu[2]->addAction(act[5]);
    menu[2]->addAction(act[10]);
    menu[2]->addAction(act[9]);

    act[7] = new QAction(QIcon(":/new/image/settings.png"),tr("更换背景"),this);
    act[8] = new QAction(QIcon(":/new/image/start.png"),tr("背景音乐"),this);
    menu[3]->addAction(act[7]);
    menu[3]->addAction(act[8]);

    menuBar = new QMenuBar(this);
    menuBar->addMenu(menu[0]);
    menuBar->addMenu(menu[1]);
    menuBar->addMenu(menu[3]);
    menuBar->addMenu(menu[2]);
    menuBar->setGeometry(0,30,1000,30);

    connect(menuBar,SIGNAL(triggered(QAction*)),this,SLOT(trigerMenu(QAction*)));

    //窗口图标、文字、背景图片设计
    QIcon icon;
    icon.addFile(QStringLiteral(":/new/image/new.ico"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    QIcon a(":/new/image/black.png");
    ui->repentchess->setIcon(a);
    ui->deposit->setIcon(a);
    ui->skip->setIcon(a);
    ui->pause->setIcon(a);
    ui->start->setIcon(a);
    ui->start_again->setIcon(a);
    //人物头像
    QMovie *movie = new QMovie;
    movie->setFileName(":/new/image/123.gif");
    ui->label_3->setMovie(movie);
    movie->start();
    ui->label_3->setScaledContents(true);

    QPixmap map3(":/new/image/black.png");
    ui->label->setPixmap(map3);
    ui->label->setScaledContents(true);
    QPixmap map4(":/new/image/white.png");
    ui->label_2->setPixmap(map4);
    ui->label_2->setScaledContents(true);
    ui->label_3->setScaledContents(true);

    QPixmap map_b("://image/boy.jpg");
    ui->label_7->setPixmap(map_b);
    ui->label_7->setScaledContents(true);

    QPixmap map_g("://image/girl.jpg");
    ui->label_6->setPixmap(map_g);
    ui->label_6->setScaledContents(true);

    w = new ChessBoard(this);
    w->setEnabled(false);

    //设置button和棋盘非使能
    ui->start->setEnabled(false);
    ui->pause->setEnabled(false);
    ui->repentchess->setEnabled(false);
    ui->skip->setEnabled(false);
    ui->deposit->setEnabled(false);
    ui->start_again->setEnabled(false);

    //设置button透明化
    ui->deposit->setFlat(true);
    ui->pause->setFlat(true);
    ui->repentchess->setFlat(true);
    ui->skip->setFlat(true);
    ui->start->setFlat(true);
    ui->start_again->setFlat(true);

    //定时器
    timer = new QTimer(this);
    t = 20;
    ui->lcdNumberClock->display(t);
    connect(timer,SIGNAL(timeout()),this,SLOT(mySlot()));
    connect(w,SIGNAL(control_clock()),this,SLOT(time_return()));

    //背景音乐设置
    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("./paiwei.mp3"));
    player->setVolume(100);

    //背景图片实现切换
    background_name = (char *)":/new/image/beijing7.jpg";

    //胜利后的音频
    play1 = new QMediaPlayer;
    play1->setMedia(QUrl::fromLocalFile("./0004.mp3"));
    play1->setVolume(80);

    //时间提示
    play2 = new QMediaPlayer;
    play2->setMedia(QUrl::fromLocalFile("./99.mp3"));
    play2->setVolume(80);

    //投降
    play4 = new QMediaPlayer;
    play4->setMedia(QUrl::fromLocalFile("./0003.wav"));
    play4->setVolume(80);

    //数据库界面
    sql = new Sql();

    // FramelessWindowHint属性设置窗口去除边框;
    // WindowMinimizeButtonHint 属性设置在窗口最小化时，点击任务栏窗口可以显示出原窗口;
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    // 设置窗口背景透明;
    //setAttribute(Qt::WA_TranslucentBackground);
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
    m_titleBar->setTitleContent(QStringLiteral("\t\t\t\t\t\t欢迎来到黑白棋界面"));
    m_titleBar->setButtonType(MIN_MAX_BUTTON);
    m_titleBar->setTitleWidth(this->width());

}

//静态全局初始化
int Form::deposit_flag = 0;
int Form::movie_flag = 0;
int Form::song_flag = 0;
int Form::background_flag = 0;
int Form::button_flag = 0;

Form::~Form()
{
    delete ui;
    delete w;
    delete timer;
    delete player;
    delete play1;
    delete play2;
    delete play4;
    delete m_titleBar;
}

//用来控制时钟显示的信号槽函数（实现计时）
void Form::mySlot()
{
    t--;
    if(t<=5&&t>=0)
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->lcdNumberClock->setPalette(pa);
        play2->play();
    }
    if((t<=17)&&(w->loop==1)&&(w->flag==1))  //设置人机下棋时间2s
    {
        if(deposit_flag == 0)  //白子托管
        {
            w->ptom_show_fun(); //人机对战显示
        }
    }
    else if((t<=17)&&(w->loop==0)&&(w->flag==1))
    {
        if(deposit_flag==1)   //黑子托管
        {
            w->black_deposit(); //黑棋托管
        }
    }
    if(t==-1)  //当时间跳过0以后，计时回归20，并实现黑白子切换（弃权作用）
    {
        t=20;
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::blue);
        ui->lcdNumberClock->setPalette(pa);
        if(w->loop==0)
        {
            w->loop = 1;
            w->setCursor(QCursor(QPixmap(":/new/image/sswhite.bmp")));
        }
        else if(w->loop==1)
        {
            w->loop = 0;
            w->setCursor(QCursor(QPixmap(":/new/image/ssblack.bmp")));
        }
    }
    ui->lcdNumberClock->display(t);
    get_chess_num();
    this->update();
}

//信号槽实现函数，用来黑白棋落子后控制时钟
void Form::time_return()
{
    t=20;
    ui->lcdNumberClock->display(t);
}


//菜单栏响应
void Form::trigerMenu(QAction *act)
{
    if(act->text() == "版本号")
    {
        QMessageBox::information(this,("提示"),
                                 ("当前版本为2.2.1"));
    }
    else if(act->text() == "玩法介绍")
    {
        if(movie_flag == 0)
        {
            In = new MediaPlayer();
            In->show();
            movie_flag = 1;
        }
        else if(movie_flag == 1)
        {
            In->player->stop();
            In->player->setVolume(0);
            In->close();
            movie_flag = 0;
        }
    }
    else if(act->text() == "退出")
    {
        int temp = QMessageBox::question(this,"Ques","确定要退出？",QMessageBox::Yes,QMessageBox::No);
        if(temp==QMessageBox::Yes)
        {
            timer->stop();
            this->close();
        }
        else if(temp==QMessageBox::No)
        {
            return;
        }
    }
    else if(act->text() == "返回上级")
    {
        this->close();
        emit mySignal();
    }
    else if(act->text() == "入门")
    {
        ui->start->setText("开始");
        m_titleBar->setTitleContent(QStringLiteral("\t\t\t\t\t\t当前模式：人机对战（入门）"));
        button_flag = 0;
        ui->start->setEnabled(true);
        ui->start_again->setEnabled(true);
        ui->deposit->setEnabled(false);
        ui->repentchess->setEnabled(true);
        ui->skip->setEnabled(true);
        w->flag = 1;
        w->gg = 0;
    }
    else if(act->text() == "简单")
    {
        ui->start->setText("开始");
        m_titleBar->setTitleContent(QStringLiteral("\t\t\t\t\t\t当前模式：人机对战（简单）"));
        button_flag = 0;
        ui->start->setEnabled(true);
        ui->deposit->setEnabled(false);
        ui->start_again->setEnabled(true);
        ui->repentchess->setEnabled(true);
        ui->skip->setEnabled(true);
        w->flag = 1;
        w->gg = 1;
    }
    else if(act->text() == "困难")
    {
        ui->start->setText("开始");
        m_titleBar->setTitleContent(QStringLiteral("\t\t\t\t\t\t当前模式：人机对战（困难）"));
        button_flag = 0;
        ui->start->setEnabled(true);
        ui->deposit->setEnabled(false);
        ui->repentchess->setEnabled(true);
        ui->start_again->setEnabled(true);
        ui->skip->setEnabled(true);
        w->flag = 1;
        w->gg = 2;
    }
    else if(act->text() == "单机")
    {
        m_titleBar->setTitleContent(QStringLiteral("\t\t\t\t\t\t当前模式：人人对战（单机）"));
        ui->start->setEnabled(true);
        ui->deposit->setEnabled(true);
        ui->repentchess->setEnabled(true);
        ui->skip->setEnabled(true);
        ui->start_again->setEnabled(true);
        ui->start->setText("开始");
        button_flag = 0;
        w->flag = 0;
    }
    else if(act->text() == "联机")
    {
        this->close();
        sql->show();
    }
    else if(act->text() == "更换背景")
    {
        if(background_flag==0)
        {
            background_flag=1;
            background_name = (char *)":/new/image/beijing6.jpeg";
            this->update();
        }
        else if(background_flag==1)
        {
            background_flag=2;
            background_name = (char *)":/new/image/beijing2.jpeg";
            this->update();
        }
        else if(background_flag==2)
        {
            background_flag=0;
            background_name = (char *)":/new/image/beijing7.jpg";
            this->update();
        }
    }
    else if(act->text() == "背景音乐")
    {
        if(song_flag==0)
        {
            player->play();
            song_flag=1;
            player->setVolume(80);

        }
        else if(song_flag==1)
        {
            player->setVolume(0);
            song_flag=0;
        }
    }
}

//绘图事件，完成设置背景色，以及黑白棋在棋盘上的打印
void Form::paintEvent(QPaintEvent *)
{
    int gw = this->width();
    int gh = this->height();
    int wx_game = (gw - 320)/2;
    int wy_game = (gh - 320)/2;

    QPainter p(this);
    p.drawPixmap(0,0,gw,gh,QPixmap(background_name));
    w->setGeometry(wx_game,wy_game,310,310); //设置棋盘在当前窗口的位置,随当前窗口的变化而变化

    this->setCursor(QCursor(QPixmap(":/new/image/777.png")));

    //谁下子，谁的头像出来
    if(w->loop==0)
    {
        ui->label_7->show();
        ui->label_6->hide();
    }
    else if(w->loop==1)
    {
        ui->label_6->show();
        ui->label_7->hide();
    }
}

//获得并显示黑白棋在棋盘上的数目并根据数目判断棋局是否完成
void Form::get_chess_num()
{
    ui->lcdNumberBlack->display(w->count_black);
    ui->lcdNumberWhite->display(w->count_white);
    if(w->count_black + w->count_white == 64)  //棋盘满了，结束
    {
        if(w->count_black > w->count_white)   //黑子多于白子
        {
            play1->play();
            int mes = QMessageBox::information(this,
                                               "Info",
                                               QObject::tr("Game Over!\n黑方胜！！！"));
            if(mes == QMessageBox::Ok)
            {
                timer->stop();
                play1->stop();
            }
        }
        else if(w->count_black <= w->count_white) //白子多于黑子
        {
            play1->play();
            int mes = QMessageBox::information(this,
                                               "Info",
                                               QObject::tr("Game Over!\n白方胜！！！"));
            if(mes == QMessageBox::Ok)
            {
                timer->stop();
                play1->stop();
            }
        }
    }
    else if((w->count_black == 0)&&( w->count_white !=0) ) //只剩下白子
    {
        play1->play();
        int mes = QMessageBox::information(this,
                                           "Info",
                                           QObject::tr("Game Over!\n白方胜！！！"));
        if(mes == QMessageBox::Ok)
        {
            timer->stop();
            play1->stop();
        }
    }
    else if((w->count_white == 0) && (w->count_black != 0))  //只剩下黑子
    {
        play1->play();
        int mes = QMessageBox::information(this,
                                           "Info",
                                           QObject::tr("Game Over!\n黑方胜！！！"));
        if(mes == QMessageBox::Ok)
        {
            timer->stop();
            play1->stop();
        }
    }
    else if((w->loop)==1&&(w->down_chess)==3)
    {
        int mes = QMessageBox::information(this,
                                           "Info",
                                           QObject::tr("Game Over!\n白方没地方可下，黑方胜！！！"));
        if(mes == QMessageBox::Ok)
        {
            timer->stop();
        }

    }
    else if((w->loop)==0&&(w->down_chess)==3)
    {

        int mes = QMessageBox::information(this,
                                           "Info",
                                           QObject::tr("Game Over!\n黑方没地方可下，白方胜！！！"));
        if(mes == QMessageBox::Ok)
        {
            timer->stop();
        }
    }
    //qDebug()<<"down_chess=="<<w->down_chess;
}

//开始和暂停
void Form::on_start_clicked()
{
    if(button_flag==0)
    {
        m_titleBar->setTitleContent(QStringLiteral("\t\t\t\t\t\t欢迎来到黑白棋界面"));
        ui->start->setText("暂停");
        ui->pause->setEnabled(true);
        w->setEnabled(true);
        timer->start(1000);
        button_flag = 1;
    }
    else if(button_flag==1)
    {
        m_titleBar->setTitleContent(QStringLiteral("\t\t\t\t\t\t游戏暂停成功"));
        ui->start->setText("开始");
        ui->pause->setEnabled(false);
        w->setEnabled(false);
        player->setVolume(0);
        timer->stop();
        button_flag = 0;

    }
}

//投降
void Form::on_pause_clicked()
{
    int temp = QMessageBox::question(this,"Ques","确定要投降？",
                                     QMessageBox::Yes,QMessageBox::No);
    if(temp==QMessageBox::Yes)
    {
        play4->play();
        if(w->loop==0)
        {
            int mes = QMessageBox::information(this,
                                               "Info",
                                               QObject::tr("Game Over!\n白方胜！！！"));
            if(mes == QMessageBox::Ok)
            {
                timer->stop();
                play4->stop();
                w->setEnabled(false);
            }
        }
        else if(w->loop==1)
        {
            int mes = QMessageBox::information(this,
                                               "Info",
                                               QObject::tr("Game Over!\n黑方胜！！！"));
            if(mes == QMessageBox::Ok)
            {
                timer->stop();
                play4->stop();
                w->setEnabled(false);
            }
        }
    }
    else if(temp==QMessageBox::No)
    {
        return;
    }
}

//悔棋
void Form::on_repentchess_clicked()
{
    t=20;
    ui->lcdNumberClock->display(t);
    if(w->flag==0)        //人人对战
    {
        if(w->loop==0) //黑棋
        {
            w->loop=1;
            w->setCursor(QCursor(QPixmap(":/new/image/sswhite.bmp"))); //鼠标光标成白子
        }
        else if(w->loop==1) //白旗
        {
            w->loop = 0;
            w->setCursor(QCursor(QPixmap(":/new/image/ssblack.bmp"))); //鼠标光标成黑子
        }
        page--; //用来判断步数
    }
    if(w->flag==1)      //人机对战
    {
        w->setCursor(QCursor(QPixmap(":/new/image/ssblack.bmp")));
        page = page-2; //由于是人机、因此悔棋得悔掉两步
    }
    int i,j;
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            w->chess[i][j]=w->chess_regret[page][i][j];//将悔棋传回棋盘数组里
        }

    }
    if(page<0)    //棋盘上只剩下刚开始的四个子
    {
        page = 0;
        QMessageBox::warning(this,"warn",QObject::tr("已不能再悔棋！"));

    }
    this->w->update();

}

//跳棋
void Form::on_skip_clicked()
{

    if(w->flag==0)       //人人对战
    {
        if(w->loop==0)
        {
            w->loop=1;
            w->setCursor(QCursor(QPixmap(":/new/image/sswhite.bmp")));
        }
        else if(w->loop==1)
        {
            w->loop = 0;
            w->setCursor(QCursor(QPixmap(":/new/image/ssblack.bmp")));
        }
    }
    if(w->flag==1)       //人机对战
    {
        w->loop = 1;
    }
    // t=1;
    //  this->mySlot();
    w->update();
}
//再来一局
void Form::on_start_again_clicked()
{
    m_titleBar->setTitleContent(QStringLiteral("\t\t\t\t\t\t欢迎来到黑白棋界面"));
    ui->label_7->hide();
    ui->label_6->hide();
    w->setEnabled(false);
    ui->start->setText("开始");
    ui->skip->setEnabled(false);
    ui->repentchess->setEnabled(false);
    ui->deposit->setEnabled(false);
    ui->start_again->setEnabled(false);
    ui->pause->setEnabled(false);
    ui->start->setEnabled(false);
    t=20;
    ui->lcdNumberClock->display(t);
    timer->stop();
    w->setCursor(QCursor(QPixmap(":/new/image/ssblack.bmp")));
    w->begin_chess(); //调用显示棋盘函数
    w->loop=0;
    ui->lcdNumberBlack->display("2");
    ui->lcdNumberWhite->display("2");
}
//托管
void Form::on_deposit_clicked()
{
    ui->deposit->setEnabled(false);
    if(w->loop==1)  //白子托管
    {
        m_titleBar->setTitleContent(QStringLiteral("\t\t\t\t\t\t白棋已经托管"));
        deposit_flag=0;
        w->flag=1;
        w->color_flag = 0;
    }
    else if(w->loop==0)  //黑棋托管
    {
        m_titleBar->setTitleContent(QStringLiteral("\t\t\t\t\t\t黑棋已经托管"));
        deposit_flag=1;
        w->flag=1;
        w->color_flag = 1;
    }

}

//窗口上方按钮响应
void Form::onButtonMinClicked()
{
    showMinimized();
}

void Form::onButtonRestoreClicked()
{
    QPoint windowPos;
    QSize windowSize;
    m_titleBar->getRestoreInfo(windowPos, windowSize);
    this->setGeometry(QRect(windowPos, windowSize));
}

void Form::onButtonMaxClicked()
{
    m_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
    QRect desktopRect = QApplication::desktop()->availableGeometry();
    QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
    setGeometry(FactRect);
}

void Form::onButtonCloseClicked()
{
    int temp = QMessageBox::question(this,"Ques","确定要退出？",QMessageBox::Yes,QMessageBox::No);
    if(temp==QMessageBox::Yes)
    {
        timer->stop();
        this->close();
    }
    else if(temp==QMessageBox::No)
    {
        return;
    }
}



