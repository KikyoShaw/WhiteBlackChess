#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QVideoWidget>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include "customslider.h"

namespace Ui {
class MediaPlayer;
}

class MediaPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MediaPlayer(QWidget *parent = 0);
    ~MediaPlayer();
    QPoint point;
    //播放视频的全局变量
    QVBoxLayout* layout_video;
    QMediaPlayer* player;
    QVideoWidget* vWidget;
    //切换视频标志位
    static int play_flag;

private slots:
    void on_pushButton_open_file_clicked();
    void on_pushButton_play_and_pause_clicked();
    void onTimerOut();
    void slider_progress_clicked();
    void slider_progress_moved();
    void slider_progress_released();
    void on_pushButton_volume_clicked();
    void slider_volume_changed();

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    Ui::MediaPlayer *ui;
};

#endif // MEDIAPLAYER_H
