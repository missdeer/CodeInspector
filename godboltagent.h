#ifndef GODBOLTAGENT_H
#define GODBOLTAGENT_H

#include <QObject>
#include <QList>
#include <QNetworkAccessManager>

struct Compiler
{
    QString id;
    QString name;
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
    QVector<char> opcodes;
    unsigned long long address;
    QString text;
    int source;
    QList<AsmLink> links;
};

typedef QList<Compiler> CompilerList;

class GodboltAgent : public QObject
{
    Q_OBJECT
public:
    explicit GodboltAgent(QObject *parent = nullptr);
    ~GodboltAgent();
    const CompilerList &getCompilerList(int index);
    void compile(const CompileInfo& ci);
    const QString& getCompileOutput() const;

    const QString& getAsmContent() const;

    const QVector<AsmItem>& getAsmItems() const;

signals:
    void compilerListRetrieved();
    void compiled();
public slots:
    void switchProgrammingLanguage(int index);

private slots:
    void onCompilerListRequestFinished();
    void onCompileRequestFinished();
private:
    QMap<int, CompilerList*> m_compilerLists;
    QNetworkAccessManager m_nam;

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
};

#endif // GODBOLTAGENT_H
