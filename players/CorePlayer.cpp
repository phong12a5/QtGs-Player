#include "CorePlayer.h"
#include <QDebug>

BasePlayer::BasePlayer(QObject* parent): QObject(parent),
    m_surface(nullptr),
    m_eState(BasePlayer::Stoped),
    m_frameIndex(0)
{

}



BasePlayer::~BasePlayer(){
    closeSurface();
    while(m_frameCircle.count()){
        delete m_frameCircle.back();
        m_frameCircle.pop_back();
    }
}

QAbstractVideoSurface* BasePlayer::videoSurface() const{
    return m_surface;
}

int BasePlayer::frameWidth()
{
    return m_frameWidth;
}

void BasePlayer::setFrameWidth(int frameWidth)
{
    if(m_frameWidth != frameWidth) {
        m_frameWidth = frameWidth;
        emit frameWidthChanged();
    }
}

int BasePlayer::frameHeight()
{
    return m_frameHeight;
}

void BasePlayer::setFrameHeight(int frameHeight)
{
    if(m_frameHeight != frameHeight) {
        m_frameHeight = frameHeight;
        emit frameHeightChanged();
    }
}

void BasePlayer::setVideoSurface(QAbstractVideoSurface* surface){
    qDebug() << __FUNCTION__;
    if (m_surface)
        closeSurface();
    m_surface = surface;
}

void BasePlayer::closeSurface(){
    if (m_surface && m_surface->isActive()){
        m_surface->stop();
        m_eState = BasePlayer::Stoped;
    }
}

void BasePlayer::setFormat(QVideoSurfaceFormat format){
    if(m_format != format) {
        m_format = format;
        setFrameWidth(format.frameSize().width());
        setFrameHeight(format.frameSize().height());
    }
}

void BasePlayer::setEState(stateE state){
    if (m_eState == state)
        return;
    m_eState = state;
    emit eStateChanged(m_eState);
}

void BasePlayer::updateFrame(QVideoFrame* frame){
    if(!frame) return;
    QVideoSurfaceFormat format = QVideoSurfaceFormat(frame->size(), frame->pixelFormat());
    setFormat(format);

    if(!m_surface->isActive()) {
        m_surface->start(m_format);
    }

    if (!m_surface->present(*frame)){
        m_eState = BasePlayer::Error;
    }
    else{
        m_eState = BasePlayer::Playing;
    }

    m_frameCircle.append(frame);
    while (m_frameCircle.size() > 15) {
        QVideoFrame* oldFrame = m_frameCircle.takeFirst();
        delete  oldFrame;
    }
}
