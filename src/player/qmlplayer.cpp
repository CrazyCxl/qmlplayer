#include "qmlplayer.h"
#include <QPainter>
#include <QTimer>

QMLPlayer::QMLPlayer(QQuickItem *parent)
    :QQuickPaintedItem(parent)
{
    player = new Player();
    player->setCallBack(this);
    QTimer *timer = new QTimer(this);
    timer->setInterval(20);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start();
}

QMLPlayer::~QMLPlayer()
{
    delete player;
}

void QMLPlayer::paint(QPainter *painter)
{
    QImage img = image.scaled(this->width(),this->height());
    painter->drawImage(QPoint(0,0),img);
}

void QMLPlayer::rowVideoData(unsigned char *data, int width, int height)
{
    image = QImage(data, width, height, QImage::Format_RGBA8888);
}

QString QMLPlayer::getUrl() const
{
    return url;
}

void QMLPlayer::setUrl(const QString &value)
{
    url = value;
}

void QMLPlayer::start()
{
    player->open("C:\\Users\\cxl\\Videos\\VID_20200416_211528.mp4");
}
