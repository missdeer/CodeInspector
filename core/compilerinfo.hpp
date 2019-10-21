#ifndef COMPILERINFO_HPP
#define COMPILERINFO_HPP

#include <QByteArray>
#include <QString>

struct CompileInfo
{
    QByteArray source;
    QString    language;
    QString    compiler;
    QString    userArguments;
    bool       labels {};
    bool       directives {};
    bool       commentOnly {};
    bool       trim {};
    bool       binary {};
    bool       intel {};
    bool       demangle {};
    bool       functions {};
};
#endif // COMPILERINFO_HPP
