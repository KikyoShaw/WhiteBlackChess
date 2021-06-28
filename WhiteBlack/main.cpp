#include "widget.h"
#include "start.h"

#include "netchess.h"
#include "sql.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("E:/QT/work/WhiteBlack/song/game.db");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qDebug()<<"open failed";
    }
    else
    {
        qDebug()<<"open success";
        qDebug()<<db.tables();
    }

//    Sql sql;
//    sql.show();


    Start s;
    s.show();

//   NetChess n;
//   n.show();

    return a.exec();
}
