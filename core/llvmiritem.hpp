#ifndef LLVMIRITEM_HPP
#define LLVMIRITEM_HPP

#include <QSharedPointer>
#include <QVector>

struct LLVMIRItem
{
    QString text;
    QString scope;
    QString sourceFile;
    QString sourceLine;
};

using LLVMIRItemPtr  = QSharedPointer<LLVMIRItem>;
using LLVMIRItemList = QVector<LLVMIRItemPtr>;

#endif
