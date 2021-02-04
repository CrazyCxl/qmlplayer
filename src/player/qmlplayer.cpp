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
    image = QImage(data, width, height, QImage::Format_RGB888);
}

void QMLPlayer::rowAudioData(unsigned char *data, unsigned int size)
{
    audioOutputIO->write((const char *)data,size);
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
    QAudioFormat format;
    // Set up the format, eg.
    format.setSampleRate(48000);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioOutput *audio_output = new QAudioOutput(format,this);
    audioOutputIO = (audio_output)->start();
    int size = audio_output->periodSize();
    player->open("C:\\Users\\cxl\\Videos\\VID_20200416_211528.mp4");
}
