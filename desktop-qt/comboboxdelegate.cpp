#include <QPainter>

#include "comboboxdelegate.h"

ComboBoxDelegate::ComboBoxDelegate(QObject *parent) : QItemDelegate(parent) {}

void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.data(Qt::AccessibleDescriptionRole).toString() == QStringLiteral("separator"))
    {
        painter->setPen(Qt::gray);
        painter->drawLine(option.rect.left(), option.rect.center().y(), option.rect.right(), option.rect.center().y());
    }
    else if (index.data(Qt::AccessibleDescriptionRole).toString() == QStringLiteral("parent"))
    {
        QStyleOptionViewItem parentOption = option;
        parentOption.state |= QStyle::State_Enabled;
        QItemDelegate::paint(painter, parentOption, index);
    }
    else if (index.data(Qt::AccessibleDescriptionRole).toString() == QStringLiteral("child"))
    {
        QStyleOptionViewItem childOption = option;
        int                  indent      = option.fontMetrics.horizontalAdvance(QString(4, QChar(' ')));
        childOption.rect.adjust(indent, 0, 0, 0);
        childOption.textElideMode = Qt::ElideNone;
        QItemDelegate::paint(painter, childOption, index);
    }
    else
    {
        QItemDelegate::paint(painter, option, index);
    }
}

QSize ComboBoxDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.data(Qt::AccessibleDescriptionRole).toString() == QStringLiteral("separator"))
    {
        return {0, 10};
    }
    return QItemDelegate::sizeHint(option, index);
}
