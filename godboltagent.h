#ifndef GODBOLTAGENT_H
#define GODBOLTAGENT_H

#include <QObject>
#include <QList>
#include <QNetworkAccessManager>

struct Language
{
    QString id;
    QString name;
    QString monaco;
    QStringList extensions;
    QStringList alias;
    QString example;
};

typedef QSharedPointer<Language> LanguagePtr;
typedef QList<LanguagePtr> LanguageList;

struct Compiler
{
    QString id;
    QString name;
    QString version;
    bool supportsBinary;
    bool supportsExecute;
    bool supportsIntel;
    bool supportsDemangle;
    bool supportsCfg;
    bool supportsAstView;
    bool supportsOptimizationOutput;
};

typedef QSharedPointer<Compiler> CompilerPtr;
typedef QList<CompilerPtr> CompilerList;
typedef QSharedPointer<CompilerList> CompilerListPtr;

struct CompileInfo
{
    CompileInfo()
        : labels(false)
        , directives(false)
        , commentOnly(false)
        , trim(false)
        , binary(false)
        , intel(false)
        , demangle(false)
    {}
    QByteArray source;
    QString language;
    QString compiler;
    QString userArguments;
    bool labels;
    bool directives;
    bool commentOnly;
    bool trim;
    bool binary;
    bool intel;
    bool demangle;
};

struct LibraryVersion
{
    QString version;
    QStringList path;
};

typedef QSharedPointer<LibraryVersion> LibraryVersionPtr;

struct Library
{
    QString id;
    QString name;
    QString description;
    QString url;
    QList<LibraryVersionPtr> versions;
};

typedef QSharedPointer<Library> LibraryPtr;
typedef QList<LibraryPtr> LibraryList;
typedef QSharedPointer<LibraryList> LibraryListPtr;

struct AsmLink
{
    AsmLink() : offset(0), length(0), to(0) {}
    int offset;
    int length;
    unsigned long long to;
};

typedef QSharedPointer<AsmLink> AsmLinkPtr;

struct AsmItem
{
    AsmItem() : address(0), source(-1) {}
    QVector<unsigned char> opcodes;
    unsigned long long address;
    QString text;
    int source;
    QList<AsmLinkPtr> links;
};

typedef QSharedPointer<AsmItem> AsmItemPtr;
typedef QVector<AsmItemPtr> AsmItemList;

class GodboltAgent : public QObject
{
    Q_OBJECT
public:
    explicit GodboltAgent(QObject *parent = nullptr);
    ~GodboltAgent();
    LanguageList &getLanguageList();
    CompilerListPtr getCompilerList(const QString& languageName);
    LibraryListPtr getLibraryList(const QString& languageName);
    void compile(const CompileInfo& ci);
    bool canCompile(const QString &language, const QString &compiler);
    const QString& getCompileOutput() const;

    const QString& getAsmContent() const;

    const AsmItemList &getAsmItems() const;

    const QString& getExample(const QString& language) const;
    const QString& getDefaultCompilerName(const QString& languageName);
signals:
    void compilerListRetrieved();
    void languageListRetrieved();
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
    bool loadConfiguration( QByteArray& content);
    bool parseConfiguration(const QByteArray& content);

    const QString& getLanguageId(const QString& name);
    const QString& getLanguageName(const QString& id);
    const QString& getCompilerId(CompilerListPtr compilerList, const QString& name);
};

#endif // GODBOLTAGENT_H
