#include "qmlplayer.h"
#include <QPainter>

QMLPlayer::QMLPlayer(QQuickItem *parent)
    :QQuickPaintedItem(parent)
{
    player = new Player();
    player->setCallBack(this);
}

QMLPlayer::~QMLPlayer()
{
    delete player;
}

void QMLPlayer::paint(QPainter *painter)
{
    painter->drawImage(QPoint(0,0),this->image);
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
