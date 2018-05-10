#ifndef GODBOLTAGENT_H
#define GODBOLTAGENT_H

#include <QObject>
#include <QList>
#include <QNetworkAccessManager>

struct Language
{
    QString id;
    QString name;
};


struct Compiler
{
    QString id;
    QString name;
    QString version;
    bool supportsBinary;
    bool supportsExecute;
    bool supportsIntel;
};

struct CompileInfo
{
    CompileInfo()
        : programmingLanguageIndex(0)
        , compilerIndex(0)
        , labels(false)
        , directives(false)
        , commentOnly(false)
        , trim(false)
        , binary(false)
        , intel(false)
    {}
    QByteArray source;
    int programmingLanguageIndex;
    int compilerIndex;
    QString userArguments;
    bool labels;
    bool directives;
    bool commentOnly;
    bool trim;
    bool binary;
    bool intel;
};

struct AsmLink
{
    AsmLink() : offset(0), length(0), to(0) {}
    int offset;
    int length;
    unsigned long long to;
};

struct AsmItem
{
    AsmItem() : address(0), source(-1) {}
    QVector<unsigned char> opcodes;
    unsigned long long address;
    QString text;
    int source;
    QList<AsmLink> links;
};

typedef QList<Language> LanguageList;
typedef QList<Compiler> CompilerList;

class GodboltAgent : public QObject
{
    Q_OBJECT
public:
    explicit GodboltAgent(QObject *parent = nullptr);
    ~GodboltAgent();
    const LanguageList &getLanguageList();
    const CompilerList &getCompilerList(int index);
    void compile(const CompileInfo& ci);
    bool canCompile(int programmingLanguageIndex, int compilerIndex);
    const QString& getCompileOutput() const;

    const QString& getAsmContent() const;

    const QVector<AsmItem>& getAsmItems() const;

signals:
    void compilerListRetrieved();
    void languageListRetrieved();
    void compiled();
public slots:
    void switchLanguage(int index);

private slots:
    void onCompilerListRequestFinished();
    void onCompileRequestFinished();
    void onLanguageListRequestFinished();
private:
    QMap<int, CompilerList*> m_compilerLists;
    QNetworkAccessManager m_nam;
    LanguageList m_languageList;

    QStringList m_backendUrls = {
        "https://gcc.godbolt.org",
        "https://d.godbolt.org",
        "https://rust.godbolt.org",
        "https://go.godbolt.org",
        "https://ispc.godbolt.org",
        "https://haskell.godbolt.org",
        "https://swift.godbolt.org",
    };

    QString m_compileOutput;
    QString m_asmContent;
    QVector<AsmItem> m_asmItems;

    bool storeCompilerList(int index, const QByteArray& content);
    bool loadCompilerList(int index, QByteArray& content);
    bool parseCompilerListFromJSON(int index, const QByteArray& content);

    bool storeLanguageList(const QByteArray& content);
    bool loadLanguageList(QByteArray& content);
    bool parseLanguageList(const QByteArray& content);
};

#endif // GODBOLTAGENT_H
