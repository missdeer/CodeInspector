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

typedef QList<Compiler> CompilerList;

class GodboltAgent : public QObject
{
    Q_OBJECT
public:
    explicit GodboltAgent(QObject *parent = nullptr);
    ~GodboltAgent();
    const CompilerList &getCompilerList(int index);
    void compile(const CompileInfo& ci);
signals:
    void compilerListRetrieved();
    void compilingFailed();
    void compiled();
public slots:
    void switchCompiler(int index);

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
};

#endif // GODBOLTAGENT_H
