#ifndef GODBOLTAGENT_H
#define GODBOLTAGENT_H

#include <QObject>
#include <QList>

struct Compiler
{
    QString id;
    QString name;
};

typedef QList<Compiler> CompilerList;

class GodboltAgent : public QObject
{
    Q_OBJECT
public:
    explicit GodboltAgent(QObject *parent = nullptr);
    ~GodboltAgent();
    const CompilerList &getCompilerList(int index);
signals:
    void compilerListRetrieved();
public slots:
    void switchCompiler(int index);

private slots:
    void onCompilerListRequestFinished();
    void onCompilerListRequestReadyRead();
    void onCompilerListRequestError(QNetworkReply::NetworkError e);
private:
    QMap<int, CompilerList*> m_compilerLists;
    QNetworkAccessManager m_nam;
    QByteArray m_response;
};

#endif // GODBOLTAGENT_H
