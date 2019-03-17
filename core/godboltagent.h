#ifndef GODBOLTAGENT_H
#define GODBOLTAGENT_H

#include <QObject>
#include <QList>
#include <QNetworkAccessManager>
#include "library.h"

struct Language
{
    QString id;
    QString name;
    QString monaco;
    QStringList extensions;
    QStringList alias;
    QString example;
};

using LanguagePtr = QSharedPointer<Language>;
using LanguageList = QList<LanguagePtr>;

struct Compiler
{
    QString id;
    QString name;
    QString version;
    QString includeFlag;
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

struct AsmLink
{
    int offset{};
    int length{};
    unsigned long long to{};
};

using AsmLinkPtr = QSharedPointer<AsmLink>;

struct AsmItem
{
    QVector<unsigned char> opcodes;
    unsigned long long address{};
    QString text;
    int source{-1};
    QList<AsmLinkPtr> links;
};

using AsmItemPtr = QSharedPointer<AsmItem>;
using AsmItemList = QVector<AsmItemPtr>;

class GodboltAgent : public QObject
{
    Q_OBJECT
public:
    explicit GodboltAgent(QObject *parent = nullptr);
    GodboltAgent(const GodboltAgent&) = delete;
    void operator=(const GodboltAgent&) = delete;
    GodboltAgent(GodboltAgent&&) = delete;
    void operator=(GodboltAgent&&) = delete;
    ~GodboltAgent() override = default;

    void initialize();
    LanguageList &getLanguageList();
    CompilerListPtr getCompilerList(const QString& languageName);
    LibraryListPtr getLibraryList(const QString& languageName);
    CompilerPtr getCompiler(const QString& language, const QString& compiler);
    void compile(const CompileInfo& ci);
    bool canCompile(const QString &language, const QString &compiler);
    const QString& getCompileOutput() const;
    const QString& getAsmContent() const;
    const AsmItemList &getAsmItems() const;
    const QString& getExample(const QString& language) const;
    const QString& getDefaultCompilerName(const QString& languageName);
signals:
    void compilerListReady();
    void languageListReady();
    void configurationReady();
    void compiled();
public slots:
    void switchLanguage(const QString& name);
private slots:
    void onCompilerListRequestFinished();
    void onCompileRequestFinished();
    void onLanguageListRequestFinished();
    void onConfigurationRequestFinished();
private:
    QNetworkAccessManager m_nam;
    LanguageList m_languageList;
    QString m_compileOutput;
    QString m_asmContent;
    AsmItemList m_asmItems;
    QMap<QString, CompilerListPtr> m_compilerMap; // language name - compiler list
    QMap<QString, QString> m_defaultCompiler; // language id - compiler id
    QMap<QString, LibraryListPtr> m_libs; // language id - library list

    void requestCompilerList(const QString &language);
    bool storeCompilerList(const QString& name, const QByteArray& content);
    bool loadCompilerList(const QString& name, QByteArray& content);
    bool parseCompilerListFromJSON(const QString &language, const QByteArray& content);
    bool parseCompilerListFromConfiguration(QJsonArray& array);

    void requestLanguageList();
    bool storeLanguageList(const QByteArray& content);
    bool loadLanguageList(QByteArray& content);
    bool parseLanguageListFromJSON(const QByteArray& content);
    bool parseLanguageListFromConfiguration(QJsonObject& obj);

    bool parseLibListFromConfiguration(QJsonObject& obj);
    bool parseDefaultCompilerFromConfiguration(QJsonObject& obj);

    void requestConfigurations();
    bool storeConfiguration(const QByteArray& content);
    bool loadConfiguration(const QString &path, QByteArray& content);
    bool parseConfiguration(const QByteArray& content);

    const QString& getLanguageId(const QString& name);
    const QString& getLanguageName(const QString& id);
    const QString& getCompilerId(CompilerListPtr compilerList, const QString& name);
};

#endif // GODBOLTAGENT_H
