#pragma once

class QAbstractItemModel;
class QObject;

namespace GroupView
{
void registerTypes();
QAbstractItemModel *makeProxy(QAbstractItemModel *model, QObject *parent = nullptr);
}
