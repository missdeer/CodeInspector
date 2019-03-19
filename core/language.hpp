#ifndef LANGUAGE_HPP
#define LANGUAGE_HPP

#include <QString>
#include <QSharedPointer>
#include <QList>

struct Language
{
    QString id;
    QString name;
    QString monaco;
    QStringList extensions;
    QStringList alias;
    QString example;
    QString defaultCompiler;
};

using LanguagePtr = QSharedPointer<Language>;
using LanguageList = QList<LanguagePtr>;

#endif // LANGUAGE_HPP
