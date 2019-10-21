#ifndef OPTIMIZATIONITEM_HPP
#define OPTIMIZATIONITEM_HPP

#include <QSharedPointer>
#include <QVector>

struct OptimizationItem
{
    QString pass;
    QString name;
    QString function;
    QString type;
    QString display;
};

using OptimizationItemPtr     = QSharedPointer<OptimizationItem>;
using OptimizationItemList    = QVector<OptimizationItemPtr>;
using OptimizationItemListPtr = QSharedPointer<OptimizationItemList>;

#endif // OPTIMIZATIONITEM_HPP
