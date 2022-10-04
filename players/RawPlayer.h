#ifndef RAWPLAYER_H
#define RAWPLAYER_H

#include <QObject>
#include <QtQml/qqml.h>
#include "GstBinding.h"
#include <QQuickItem>

class RawPlayer : public GstVideoPlayer
{
    Q_OBJECT

public:
    explicit RawPlayer(QObject * parent = nullptr) : GstVideoPlayer(parent)
    {
        m_bpp = 2;
        setSource("filesrc location=test-videos/sintel_trailer-480p.webm ! decodebin ! videoconvert ! appsink emit-signals=true name=sink0");
        setFormat(QVideoSurfaceFormat(QSize(854,480), QVideoFrame::PixelFormat::Format_YUV420P, QAbstractVideoBuffer::NoHandle));

//        setFormat(QVideoSurfaceFormat(QSize(576,1024), QVideoFrame::PixelFormat::Format_YUV420P, QAbstractVideoBuffer::NoHandle));
//        setSource("filesrc location=test-videos/download.mp4 ! decodebin ! videoconvert ! appsink emit-signals=true name=sink0");
//        setFormat(QVideoSurfaceFormat(QSize(576,1024), QVideoFrame::Format_UYVY));
//        setSource("gst-pipeline: filesrc location=download.mp4 ! qtdemux ! avdec_h264 ! qtvideosink");
//        setSource("filesrc location=download.mp4 ! decodebin ! videoconvert ! appsink emit-signals=true name=sink0");
//        setSource("filesrc location=~/Downloads/download.mp4 ! decodebin ! videoconvert ! appsink emit-signals=true name=sink0");

//        setFormat(QVideoSurfaceFormat(QSize(856,480), QVideoFrame::PixelFormat::Format_YUV420P, QAbstractVideoBuffer::NoHandle));
//        setSource("playbin uri=https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm ! decodebin ! videoconvert ! appsink emit-signals=true name=sink0 ");
    };
};

#endif // RAWPLAYER_H
