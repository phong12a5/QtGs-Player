#include "videoplayer.h"
#include <QDebug>

VideoPlayer::VideoPlayer(QObject *parent) : QObject(parent)
{
    interface = new GST_Interface(nullptr);
}

void VideoPlayer::playMedia(QString uri)
{
    qDebug() << __FUNCTION__ << ":" << uri;
    interface->playMedia(uri);
}

void VideoPlayer::stopMedia()
{
    interface->playerStop();
}

void VideoPlayer::playPause()
{
    interface->playPause();
}
