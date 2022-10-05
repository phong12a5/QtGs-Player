#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <players/videoplayer.h>
#include <players/usbvideoplayer.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<UsbVideoPlayer>("UsbVideoPlayer",1,0,"UsbVideoPlayer");


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    VideoPlayer player;
    engine.rootContext()->setContextProperty("VideoPlayer", &player);

    engine.load(url);

    return app.exec();
}
