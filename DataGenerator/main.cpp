#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>
#include <QDebug>
#include "imagecollector.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<ImageCollector>("com.dataGen.ImageCollector", 1, 0, "ImageCollector");
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
        {
            qDebug() << address.toString();
            engine.rootContext()->setContextProperty(QStringLiteral("hostIp"), address.toString());
        }
    }
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
