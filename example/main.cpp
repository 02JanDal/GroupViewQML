#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

#include <GroupView.h>

#include "InstanceModel.h"

#include <QDebug>
#include <QDir>

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	Q_INIT_RESOURCE(images);

	GroupView::registerTypes();

	QQmlApplicationEngine engine;

	engine.rootContext()->setContextProperty("instancesModel", GroupView::makeProxy(new InstanceModel(&engine)));
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	return app.exec();
}
