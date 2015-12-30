#include "GroupView.h"

#include <QtQml>

#include "GroupViewDropArea_p.h"
#include "GroupViewHelper_p.h"
#include "GroupViewProxy_p.h"

void GroupView::registerTypes()
{
	qmlRegisterType<GroupViewDropArea>("xyz.jandal.groupview.internal", 1, 0, "GroupViewDropArea");
	qmlRegisterType<GroupViewHelper>("xyz.jandal.groupview.internal", 1, 0, "GroupViewHelper");
	qmlRegisterType(QUrl("qrc:/xyz/jandal/groupview/qml/GroupView.qml"), "xyz.jandal.groupview", 1, 0, "GroupView");
	qmlRegisterType(QUrl("qrc:/xyz/jandal/groupview/qml/ScrollBar.qml"), "xyz.jandal.groupview", 1, 0, "ScrollBar");
}

QAbstractItemModel *GroupView::makeProxy(QAbstractItemModel *model, QObject *parent)
{
	GroupViewProxy *proxy = new GroupViewProxy(parent);
	proxy->setSourceModel(model);
	return proxy;
}
