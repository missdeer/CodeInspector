#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <QString>
#include <QSharedPointer>
#include <QList>

struct Compiler
{
    QString id;
    QString name;
    QString version;
    QString includeFlag;
    QString group;
    QString groupName;
    bool supportsBinary;
    bool supportsExecute;
    bool supportsIntel;
    bool supportsDemangle;
    bool supportsCfg;
    bool supportsAstView;
    bool supportsOptimizationOutput;
    bool supportsGccDump;
};

using CompilerPtr = QSharedPointer<Compiler>;
using CompilerList = QList<CompilerPtr>;
using CompilerListPtr = QSharedPointer<CompilerList>;

#endif // COMPILER_HPP
