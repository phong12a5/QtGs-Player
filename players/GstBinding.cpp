#include "GstBinding.h"
#include <QDebug>

static QVideoFrame::PixelFormat gst_video_format_to_qvideoformat(GstVideoFormat format)
{
    switch (format) {
    case GST_VIDEO_FORMAT_I420:
        return QVideoFrame::PixelFormat::Format_YUV420P;
    case GST_VIDEO_FORMAT_NV12:
        return QVideoFrame::PixelFormat::Format_NV12;
    case GST_VIDEO_FORMAT_UYVY:
        return QVideoFrame::PixelFormat::Format_UYVY;
    case GST_VIDEO_FORMAT_YUY2:
        return QVideoFrame::PixelFormat::Format_YUYV;
    case GST_VIDEO_FORMAT_YV12:
        return QVideoFrame::PixelFormat::Format_YV12;
    case GST_VIDEO_FORMAT_RGB:
        return QVideoFrame::PixelFormat::Format_RGB24;
    case GST_VIDEO_FORMAT_BGR:
        return QVideoFrame::PixelFormat::Format_BGR24;
    case GST_VIDEO_FORMAT_ARGB:
        return QVideoFrame::PixelFormat::Format_ARGB32;
    case GST_VIDEO_FORMAT_RGBA:
        return QVideoFrame::PixelFormat::Format_RGB32;
    case GST_VIDEO_FORMAT_xRGB:
        return QVideoFrame::PixelFormat::Format_ARGB32;
    case GST_VIDEO_FORMAT_BGRx:
        return QVideoFrame::PixelFormat::Format_BGR32;
    case GST_VIDEO_FORMAT_GRAY8:
        return QVideoFrame::PixelFormat::Format_Y8;
    default:
        qCritical() << "Unsupported format";
    }
    return QVideoFrame::PixelFormat::Format_Invalid;
}

static GstFlowReturn OnAppsinkNewFrame(GstElement *sink, GstVideoPlayer *videoplayer)
{
    Q_UNUSED(sink)
    return (GstFlowReturn)videoplayer->pullAppsinkFrame();
}

GstVideoPlayer::GstVideoPlayer(QObject * parent) : BasePlayer(parent),
    m_pipeline(nullptr),
    m_appsink(nullptr),
    m_sinkName("sink0")
{
    gst_init(NULL, NULL);

    connect(this, &GstVideoPlayer::newFrame, this, &GstVideoPlayer::updateFrame);
    connect(this, &GstVideoPlayer::sourceChanged, this, &GstVideoPlayer::startByBtn);
    connect(this, &GstVideoPlayer::reconnect, this, &GstVideoPlayer::tryToReconnect);
    connect(this, &GstVideoPlayer::startedByBtn, this, [=](){this->setEState(BasePlayer::Playing);});
}
GstVideoPlayer::~GstVideoPlayer(){
    stop();
}

void GstVideoPlayer::tryToReconnect(){
    stop();
    start();
}

QString GstVideoPlayer::source() const{
    return m_source;
}

void GstVideoPlayer::setSource(QString source){
     if(m_source == source)
         return;
     m_source = source;
     emit sourceChanged();
}

void GstVideoPlayer::startByBtn(){
    emit startedByBtn();
    start();
}

void GstVideoPlayer::start(){
    qDebug() << "start video player";

    m_pipeline = gst_parse_launch(qPrintable(m_source),NULL);
    if (!m_pipeline){
        qCritical() << "Failed to start pipeline";
        setEState(BasePlayer::Error);
        return;
    }

    //get appsink from gstreamer pipeline
    m_appsink = gst_bin_get_by_name(GST_BIN(m_pipeline), qPrintable(m_sinkName));
    if (!m_appsink){
        qCritical() << "Failed to get appsrc";
        gst_object_unref(m_pipeline);
        setEState(BasePlayer::Error);
        return;
    }

    //connecting gstreamer signal to callback functions
    //new-sample emiting when pipeline get new frame from source
    g_signal_connect (m_appsink, "new-sample", G_CALLBACK (OnAppsinkNewFrame), this);


    //set gstreamer pipeline to playing state
    GstStateChangeReturn res = gst_element_set_state(GST_ELEMENT(m_pipeline), GST_STATE_PLAYING);
    if (res == GST_STATE_CHANGE_FAILURE)
    {
        qCritical() << "Unable to set the pipeline to the playing state";
        gst_object_unref(m_appsink);
        gst_object_unref(m_pipeline);
        setEState(BasePlayer::Error);
        return;
    }
    emit started();

}

void GstVideoPlayer::stop(){

    //set gstreamer pipelin to null state(stop state)
    if (m_pipeline)
        gst_element_set_state(GST_ELEMENT(m_pipeline), GST_STATE_NULL);

    //unref all g_object used is programm
    if (m_appsink) {
        gst_object_unref(m_appsink);
        m_appsink = nullptr;
    }
    if(m_pipeline) {
        gst_object_unref(m_pipeline);
        m_pipeline = nullptr;
    }
    qDebug() << "video player stopped";
    setEState(BasePlayer::Stoped);
    emit stopped();
}

int GstVideoPlayer::pullAppsinkFrame(){
    GstSample *sample;
    GstBuffer *buf;
    GstMapInfo Ginfo;

    g_signal_emit_by_name (m_appsink, "pull-sample", &sample);
    if (! sample) {
        qCritical() << "Cant get frame from sample";
        setEState(BasePlayer::Error);
        return GST_FLOW_ERROR;
    }

    GstCaps *caps = gst_sample_get_caps(sample);
    if(!caps) {
        qCritical() << "Cant get caps from sample";
        setEState(BasePlayer::Error);
        return GST_FLOW_ERROR;
    } else {
        GstStructure *structure = gst_caps_get_structure(caps, 0);
        const int width = g_value_get_int(gst_structure_get_value(structure, "width"));
        const int height = g_value_get_int(gst_structure_get_value(structure, "height"));
        qDebug() << "format: " << gst_structure_get_string (structure, "format");
        const int format = gst_video_format_from_string(gst_structure_get_string (structure, "format"));
        qDebug() << "width: " << width << ", height: " << height << ", format: " << format;

        GstVideoFormat videoFormat = gst_video_format_from_string (gst_structure_get_string (structure, "format"));
        const GstVideoFormatInfo * videoFormatInfo = gst_video_format_get_info (videoFormat);
        qDebug() << "pixel_stride : " << videoFormatInfo->pixel_stride ;


    }

    QVideoFrame* frame = getPtrFromFrameCircle();
    if (!frame){
        qCritical() << "no buffers in QVideoFrame pool";
        setEState(BasePlayer::Error);
        return GST_FLOW_ERROR;
    }

    buf = gst_sample_get_buffer(sample);
    if (!buf){
        qCritical() << "Unable to get buffer";
        setEState(BasePlayer::Error);
        return GST_FLOW_ERROR;
    }

    GstVideoInfo* video_info = gst_video_info_new();
    if (!gst_video_info_from_caps(video_info, caps))
    {
        // Could not parse video info (should not happen)
        g_warning("Failed to parse video info");
        return GST_FLOW_ERROR;
    }

    //https://gstreamer.freedesktop.org/documentation/video/video-frame.html?gi-language=c
    GstVideoFrame videoFrame;
    if(!gst_video_frame_map (&videoFrame, video_info, buf, GST_MAP_READ)) {
        g_warning("Failed to videoFrame");
    } else {
//        qDebug() << videoFrame.
    }

    if (!gst_buffer_map(buf,&Ginfo, GST_MAP_READ)){
        qCritical() << "unable to map GstBuffer";
        gst_sample_unref(sample);
        setEState(BasePlayer::Error);
        return GST_FLOW_ERROR;
    }


    if (frame->isMapped())
        frame->unmap();


    if (!frame->map(QAbstractVideoBuffer::ReadWrite)){
        qCritical() << "Unable to map QVideoFrame";
        gst_buffer_unmap(buf,&Ginfo);
        gst_sample_unref (sample);
        setEState(BasePlayer::Error);
        return GST_FLOW_ERROR;
    }

    //check if source format and frame format match
//    //qDebug() << info.size;
    if (frame->mappedBytes() != int(Ginfo.size)){
        qDebug() << frame->mappedBytes() << Ginfo.size;
        qCritical() << "source format and QVideoFrame format dont match";
//        frame->unmap();
//        gst_buffer_unmap(buf,&Ginfo);
//        gst_sample_unref (sample);
//        setEState(BasePlayer::Error);
//        return GST_FLOW_ERROR;
    }


    //copy data from gstreamer buffer to buffer from QVideoFrame
    memcpy(frame->bits(), Ginfo.data, Ginfo.size);

    //unmapping and release buffers
    frame->unmap();

    gst_buffer_unmap(buf,&Ginfo);
    gst_sample_unref (sample);

    emit newFrame(frame);
    return GST_FLOW_OK;
}
