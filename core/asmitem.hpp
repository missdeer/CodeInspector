#ifndef ASMITEM_HPP
#define ASMITEM_HPP

#include <QVector>

#include "asmlink.hpp"

struct AsmItem
{
    QVector<unsigned char> opcodes;
    unsigned long long     address {};
    QString                text;
    int                    source {-1};
    QList<AsmLinkPtr>      links;
};

using AsmItemPtr  = QSharedPointer<AsmItem>;
using AsmItemList = QVector<AsmItemPtr>;

#endif // ASMITEM_HPP
