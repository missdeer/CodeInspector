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
    
    bool supportsBinary{false};
    bool supportsExecute{false};
    bool supportsIntel{false};
    bool supportsDemangle{false};
    bool supportsCfg{false};
    bool supportsAstView{false};
    bool supportsOptimizationOutput{false};
    bool supportsGccDump{false};
    
    bool supportClangTidy{false};
    bool supportLLVMMCA{false};
    bool supportPahole{false};
    bool supportClangQuery{false};
    bool supportReadElf{false};
    bool supportX86To6502{false};
};

using CompilerPtr = QSharedPointer<Compiler>;
using CompilerList = QList<CompilerPtr>;
using CompilerListPtr = QSharedPointer<CompilerList>;

#endif // COMPILER_HPP
