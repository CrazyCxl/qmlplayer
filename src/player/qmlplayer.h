#ifndef QMLPLAYER_H
#define QMLPLAYER_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QAudioOutput>
#include "player.h"

class QMLPlayer : public QQuickPaintedItem,public PlayerCallBack
{
    Q_OBJECT
public:
    QMLPlayer(QQuickItem *parent = 0);
    ~QMLPlayer();

    QString getUrl() const;
    void setUrl(const QString &value);

    Q_INVOKABLE void start();

private:
    void paint(QPainter *painter) override;
    void rowVideoData(unsigned char *data, int width, int height) override;
    void rowAudioData(unsigned char *data, unsigned int size) override;

private:
    QImage image;
    QString url;
    Player *player;
    QIODevice *audioOutputIO;
};

#endif // QMLPLAYER_H
