#ifndef COMPILERINFO_HPP
#define COMPILERINFO_HPP

#include <QString>
#include <QByteArray>

struct CompileInfo
{
    QByteArray source;
    QString language;
    QString compiler;
    QString userArguments;
    bool labels{};
    bool directives{};
    bool commentOnly{};
    bool trim{};
    bool binary{};
    bool intel{};
    bool demangle{};
};
#endif // COMPILERINFO_HPP
