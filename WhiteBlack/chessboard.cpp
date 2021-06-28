#include "chessboard.h"
#include "ui_chessboard.h"

int page = 0;
int num_eat_white=0;

ChessBoard::ChessBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChessBoard)
{
    ui->setupUi(this);
    this->setMouseTracking(true); //解决不用一直按着鼠标才能判断移动坐标
    this->startX=0;
    point.setX(-1);
    point.setY(-1);
    this->setCursor(QCursor(QPixmap(":/new/image/ssblack.bmp")));//设置光标为黑棋
    for(a=0;a<8;a++)
    {
        for(b=0;b<8;b++)
        {
            chess[a][b] = 2;
        }
    }
    count_black=0;
    count_white=0;
    show_mid_fourchess();

    for(a=0;a<8;a++)
    {
        for(b=0;b<8;b++)
        {
             chess_regret[page][a][b] = chess_two[a][b] =  chess[a][b];
        }
    }

    //人机的定时器
    ttime = new QTimer();
    ttime->start(30);
    if(flag==1)
    {
        connect(ttime,SIGNAL(timeout()),this,SLOT(TTimer()));
    }

    //点击棋盘后的音频
    play = new QMediaPlayer;
    play->setMedia(QUrl::fromLocalFile("./5390.mp3"));
    play->setVolume(80);

    //吃子数量的音频(只用在人机中,人人同理)
    //5颗
    player1 = new QMediaPlayer;
    player1->setMedia(QUrl::fromLocalFile("./4.mp3"));
    player1->setVolume(80);
    //5颗以上
    player2 = new QMediaPlayer;
    player2->setMedia(QUrl::fromLocalFile("./10.mp3"));
    player2->setVolume(80);
    //4颗
    player3 = new QMediaPlayer;
    player3->setMedia(QUrl::fromLocalFile("./3.mp3"));
    player3->setVolume(80);
    //3颗
    player4 = new QMediaPlayer;
    player4->setMedia(QUrl::fromLocalFile("./2.mp3"));
    player4->setVolume(80);
    //两颗
    player5 = new QMediaPlayer;
    player5->setMedia(QUrl::fromLocalFile("./1.mp3"));
    player5->setVolume(80);


}

//静态全局初始化
int ChessBoard::flag = 0;
int ChessBoard::loop = 0;
int ChessBoard::gg = 2;
int ChessBoard::black_eat_num = 0;
int ChessBoard::white_eat_num = 0;
int ChessBoard::a =0;
int ChessBoard::b =0;
int ChessBoard::c = 0;
int ChessBoard::row = 0;
int ChessBoard::col = 0;
int ChessBoard::color_flag = 0;
int ChessBoard::down_chess = 0;

//初始化棋盘
void ChessBoard::begin_chess()
{
    for(a=0;a<8;a++)
    {
        for(b=0;b<8;b++)
        {
            chess[a][b]=2;
        }
    }
    count_black=0;
    count_white=0;
    show_mid_fourchess();
}

//人机对战白棋的位置及打印函数
void ChessBoard::ptom_show_fun()
{
    ccurrentRole = White;
    if(chess[a][b]!=2) //判断鼠标点击的(a,b)处是否是空,不是空就返回,是空就继续
    {
        return;
    }
    people_pk_machine(ccurrentRole);
    //获取吃子个数
    int white_eat_nnum = judgeRule(row, col, 8, chess, ccurrentRole);
    if(white_eat_nnum==0)  //吃子为零,即点击的该格不符合要求,继续用黑棋判断下一格
    {
        chess[row][col] = Empty;
        loop = 1;
    }
    else //吃子成功
    {
        emit control_clock(); //发送时钟控制信号
        setCursor(QCursor(QPixmap(":/new/image/ssblack.bmp")));
        chess[row][col] = 1;
        loop=0;

    }
}

//人机对战显示
void ChessBoard::show_black_deposit()
{
    if(chess[a][b]!=2)
        return;
    people_pk_machine(ccurrentRole);
    int white_eat_nnum = judgeRule(row, col, 8, chess, ccurrentRole);
    if(white_eat_nnum==0)
    {
        chess[row][col]=Empty;
        loop=1;
    }
    else
    {
        emit control_clock();
        setCursor(QCursor(QPixmap(":/new/image/ssblack.bmp")));
        chess[row][col] = 1;
        loop=0;

    }
}

ChessBoard::~ChessBoard()
{
    delete ui;
    delete ttime;
    delete play;
    delete player1;
    delete player2;
    delete player3;
    delete player4;
    delete player5;
}

//人机对战的时钟函数
void ChessBoard::TTimer()
{
    if(flag == 1)
    {
        jude_chess_ptom(); //人机对战
        this->update();
    }
}

//记录每下一步棋后棋盘中黑白棋的状况，用来悔棋
void ChessBoard::voluation_three_array()
{
    for(a=0;a<8;a++)
     {
         for(b=0;b<8;b++)
         {
             chess_regret[page][a][b] = chess[a][b];
         }
     }
}

//画图事件，主要是画棋盘以及棋盘上黑白子的位置描绘
void ChessBoard::paintEvent(QPaintEvent *)
{
    bw = 314;  //固定棋盘大小
    bh = 314;
    x = (int)(bw-10)/8;  //从（5,5）开始画线
    y = (int)(bh-10)/8;

    QPainter p(this);
    p.drawPixmap(0,0,bw,bh,QPixmap(":/new/image/beijing.jpg")); //先要设置背景色，否则会被覆盖

    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(3);
    pen.setStyle(Qt::SolidLine);
    p.setPen(pen);

    for(int i=0;i<9;i++)
    {
        p.drawLine(5,5+y*i,8*x,5+y*i);
        p.drawLine(5+x*i,5,5+x*i,8*y);
    }

    //显示棋子在棋盘中的位置以及颜色，记录棋盘上黑白子的数目
    QPainter p2(this);
    count_black = 0;
    count_white = 0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(chess[i][j] == 0)
            {
                p2.drawPixmap(5+i*x,5+j*y,x,y,QPixmap(":/new/image/black.png"));
                count_black++;
            }
            if(chess[i][j] == 1)
            {
                p2.drawPixmap(5+i*x,5+j*y,x,y,QPixmap(":/new/image/white.png"));
                count_white++;

            }
        }
     }

}

//人人对战基本操作
void ChessBoard::jude_chess_ptop()
{
    if(chess[a][b]!=2)
        return;
    switch(loop)
    {
            case 0:
            {
                black_eat_num = judgeRule(a, b, 8, chess, Black);
                if(black_eat_num == 0)
                {
                    chess[a][b] = Empty;
                    loop = 0;
                    break;
                }
                else
                {
                   this->setCursor(QCursor(QPixmap(":/new/image/sswhite.bmp")));
                   loop = 1;
                   page++; //步数加1
                   voluation_three_array(); //记步数
                   emit control_clock();
                   break;
                }

            }
            case 1:
            {
                white_eat_num = judgeRule(a, b, 8, chess, White) ;
                if(white_eat_num == 0)
                {
                    chess[a][b] = Empty;
                    loop = 1;
                    break;
                }
                else
                {
                    this->setCursor(QCursor(QPixmap(":/new/image/ssblack.bmp")));
                    loop = 0;
                    page++;
                    voluation_three_array();
                    emit control_clock();
                    break;
                }

            }
        }
}

//人机对战的遍历棋盘函数，找出白棋自动落子的位置(第几格)
void ChessBoard::people_pk_machine(GridState ccurrentRole)
{
   // int num_eat_white=0;
    int k =0;
    int cb[64]; //存放白子能够落子的格数(第几格)
    memset(cb,0,sizeof(cb));
    int u=0;
    int numbers[64];
    int num_max = 0;  //记录吃子的最大数
    int num_min = 8;  //记录吃子的最小数
    memset(numbers,0,sizeof(numbers));
    int i, j;
    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
            chess_two[i][j] = chess[i][j]; //记录当前的棋盘状况
        }
    }
    for(int ii=0;ii<8;ii++)
    {
        for(int jj=0;jj<8;jj++)
        {
            num_eat_white = judgeRule(ii,jj,8,chess_two,ccurrentRole);
            if(num_eat_white>0)
            {
                numbers[k] = num_eat_white;
                c=ii+jj*8;
                cb[k]=c;
                k++;

            }
            for(i = 0; i < 8; i++)
            {
                for(j = 0; j < 8; j++)
                {
                    chess_two[i][j] = chess[i][j];
                }
            }
        }
    }

    num_max = numbers[0];
    num_min = numbers[0];
    change = cb[0];
    if(gg==0)
    {
         for(u=1;u<64;u++)
         {
             if(numbers[u] < num_min&&numbers[u]>0 ) //选取最小的吃子方式
             {
                 num_min = numbers[u];
                 change = cb[u];
             }

         }
    }
    if(gg==1)
    {
        change = cb[0];//将每次判断出来的格数组成的数组的第一个元素用来自动放置白棋(随机性)
    }
    if(gg==2)
    {
        for(u=1;u<64;u++)
        {
            if(numbers[u]>=num_max) //选取最多的吃子方式
            {
                num_max = numbers[u];
                change = cb[u];
            }
        }

    }
    row = change%8; //该格的横坐标
    col = change/8; //该格的纵坐标
}

//黑棋托管时，黑棋自动下棋
void ChessBoard::black_deposit()
{
    ccurrentRole = Black;
     if(chess[a][b] != 2)
          return;
     people_pk_machine(ccurrentRole);
     int black_eat_nnum = judgeRule(row, col, 8, chess, ccurrentRole) ;
     if(black_eat_nnum == 0)
     {
         chess[row][col] = Empty;
         loop = 0;
     }
     else
     {
         emit control_clock();
         setCursor(QCursor(QPixmap(":/new/image/sswhite.bmp")));
         chess[row][col] = 0;
         loop = 1;
     }
}

//人机对战的基本操作
void ChessBoard::jude_chess_ptom()
{
    if(chess[a][b] != 2)
        return;
    if(color_flag == 0)
    {
        if(loop == 0 )
        {
            int black_eat_nnum = judgeRule(a, b, 8, chess, Black);
            if(black_eat_nnum == 0)
            {
                chess[a][b] = Empty;
                loop = 0;
            }
            else
            {
                page++;
                voluation_three_array();
                emit control_clock();
                this->setCursor(QCursor(QPixmap(":/new/image/sswhite.bmp")));
                loop = 1;
            }
            if(black_eat_nnum == 5)
            {
                player1->play();
            }
            else if(black_eat_nnum==4)
            {
                player3->play();
            }
            else if(black_eat_nnum==3)
            {
                player4->play();
            }
            else if(black_eat_nnum==2)
            {
                player5->play();
            }
            else if(black_eat_nnum>5)
            {
                player2->play();
            }
        }
     }
    if(color_flag == 1)
    {
       if(loop == 1 )
       {
           int white_eat_nnum = judgeRule(a, b, 8, chess, White);
           if(white_eat_nnum == 0)
           {
               chess[a][b] = Empty;
               loop = 1;
           }
           else
           {
               page++;
               voluation_three_array();
               emit control_clock();
               this->setCursor(QCursor(QPixmap(":/new/image/ssblack.bmp")));
               loop = 0;
           }
       }
    }
   this->update();
}

//鼠标点击事件，获得鼠标点击棋盘的坐标并操作
void ChessBoard::mousePressEvent(QMouseEvent *e)
{
    count_black=0;
    count_white=0;
    play->play();
    QString str;
    int bb = e->button();
    if(bb==Qt::LeftButton) //左击
    {
        //鼠标点击得到的坐标
        px=e->x();
        py=e->y();
        str = QString("%1,%2").arg(px).arg(py);
        Getchess();
    }
    point.setX(e->x()-(x/2));  //将鼠标设置在图片的中间，根据上面设置图片的位置和大小来改变
    point.setY(e->y()-(y/2));
    if(flag==0)
    {
        jude_chess_ptop();
    }
    if(flag==1)
    {
        TTimer();
    }
    this->update();
}

//获得棋子的坐标
void ChessBoard::Getchess()
{
    for(a=0;a<8;a++)
     {
         if(px >= 5+x*a && px <= 5+x*(a+1))
         {
             mid_x = 5+x*a;  //得到点击处的横坐标,记得加5(原点是从(5,5)处开始画棋盘)
             break;
         }
     }
     for(b=0;b<8;b++)
     {
         if(py >= 5+y*b && py <= 5+y*(b+1))
         {
             mid_y = 5+y*b; //得到点击处的纵坐标,记得加5(原点是从(5,5)处开始画棋盘)
             break;
         }
     }
}

//获得棋盘中四颗棋子
void ChessBoard::show_mid_fourchess()
{
    chess[3][3] = 0;
    chess[3][4] = 1;
    chess[4][3] = 1;
    chess[4][4] = 0;
    this->update();
}

//吃子函数，通过吃子数目来判定是否落子
int ChessBoard::judgeRule(int x, int y, int gridNum, void *chess, GridState currentRole)
{
    int dir[8][2]={{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    if( x < 0 || x>= gridNum || y < 0 || y >= gridNum)
                    return 0;
    int temp_x = x, temp_y = y;
    int i = 0, eatNum = 0;
    typedef int (*p) [gridNum];
    p chessBoard = p(chess);
    if(chessBoard[temp_x][temp_y] != Empty)
        return 0;
    //循环判断8个方向
    for(i = 0 ; i <8; i++)
    {
        //准备判断相邻棋子
        temp_x += dir[i][0];
        temp_y += dir[i][1];
        //如果没有出界，且相邻棋子是对方棋子，才有吃子的可能．
        if((temp_x < gridNum && temp_x >=0 && temp_y < gridNum && temp_y >= 0)
                && (chessBoard[temp_x][temp_y] == !currentRole))
        {
            //继续判断下一个
            temp_x += dir[i][0];
            temp_y += dir[i][1];
            //同样判断是否出界
            while(temp_x < gridNum && temp_x >=0 && temp_y < gridNum && temp_y >= 0)
            {
                //遇到空位跳出
                if(chessBoard[temp_x][temp_y] == Empty)
                     break;
                //找到了自己的棋子，可以吃子
                if(chessBoard[temp_x][temp_y] == currentRole)
                {
                     chessBoard[x][y] = currentRole; //改变（x,y）坐标内的信息
                     temp_x -= dir[i][0];
                     temp_y -= dir[i][1];
                    //吃子的过程
                     while((temp_x != x )||(temp_y != y))
                     {
                            chessBoard[temp_x][temp_y] = currentRole;
                            temp_x -= dir[i][0];
                            temp_y -= dir[i][1];
                            eatNum ++;
                      }
                      break;
                  }
                //如果仍然是对方棋子，继续沿此方向查找
                  temp_x += dir[i][0];
                  temp_y += dir[i][1];
             }
         }
        //一个方向判断结束，准备下一个方向
          temp_x = x;
          temp_y = y;
    }
    return eatNum;
}





