#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <players/videoplayer.h>
#include <players/RawPlayer.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<RawPlayer>("RawPlayer",1,0,"RawPlayer");


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
