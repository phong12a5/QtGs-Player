#ifndef VIDEOPALYER_H
#define VIDEOPALYER_H

#include <QObject>
#include <gstiface/gstiface.h>

class VideoPlayer : public QObject
{
    Q_OBJECT
public:
    explicit VideoPlayer(QObject *parent = nullptr);

    Q_INVOKABLE void playMedia(QString uri);
    Q_INVOKABLE void stopMedia();
    Q_INVOKABLE void playPause();

signals:

private:
    GST_Interface* interface;

};

#endif // VIDEOPALYER_H
