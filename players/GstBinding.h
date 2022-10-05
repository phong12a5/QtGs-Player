#ifndef GSTBINDING_H
#define GSTBINDING_H

#include <gst/gst.h>
#include <gst/video/gstvideometa.h>
#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/video/video-info.h>
#include <gst/video/gstvideofilter.h>
#include <QObject>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QThread>
#include "CorePlayer.h"
#include <QDir>
#include <QFile>
#include <QRandomGenerator>
#include <gst/allocators/allocators.h>

class GstVideoPlayer : public BasePlayer
{
    Q_OBJECT

    Q_PROPERTY(QString source READ source WRITE setSource)

public:
    explicit GstVideoPlayer( QObject * parent = 0);
    ~GstVideoPlayer();

    int pullAppsinkFrame();

public slots:

    void startByBtn();

    void start();

    void stop();

    void playPause();

    QString source() const;

    void setSource(QString source);

protected:

protected slots:

signals:
    void started();
    void stopped();
    void newFrame(QVideoFrame *frame);
    void startedByBtn();
    void sourceChanged();

private slots:
    void tryToReconnect();

private:
    GstElement * m_pipeline;
    GstElement * m_appsink;
    QString m_launchstring;
    QString m_sinkName;
    ulong m_lastTimestamp;

protected:
    QString m_source;
};


#endif // GSTBINDING_H
