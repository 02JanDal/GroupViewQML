/* Copyright 2015 Jan Dalheimer <jan@dalheimer.de>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
