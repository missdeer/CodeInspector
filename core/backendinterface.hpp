#ifndef BACKENDINTERFACE_HPP
#define BACKENDINTERFACE_HPP

#include "compiler.hpp"
#include "language.hpp"
#include "library.h"

class BackendInterface
{
public:
    virtual ~BackendInterface()                                                              = default;
    virtual LanguageList &  getLanguageList()                                                = 0;
    virtual CompilerListPtr getCompilerList(const QString &languageName)                     = 0;
    virtual LibraryListPtr  getLibraryList(const QString &languageName)                      = 0;
    virtual CompilerPtr     getCompiler(const QString &language, const QString &compiler)    = 0;
    virtual const QString & getExample(const QString &language) const                        = 0;
    virtual const QString & getDefaultCompilerName(const QString &languageName)              = 0;
    virtual bool            canCompile(const QString &language, const QString &compiler)     = 0;
    virtual const QString & getCompilerId(CompilerListPtr compilerList, const QString &name) = 0;
};

#endif // BACKENDINTERFACE_HPP
