#ifndef GODBOLTAGENT_H
#define GODBOLTAGENT_H

#include <QObject>
#include <QList>
#include <QNetworkAccessManager>
#include "library.h"
#include "compilerinfo.hpp"
#include "asmitem.hpp"

class BackendInterface;

class GodboltAgent : public QObject
{
    Q_OBJECT
public:
    explicit GodboltAgent(QNetworkAccessManager& nam, QObject *parent = nullptr);
    GodboltAgent(const GodboltAgent&) = delete;
    void operator=(const GodboltAgent&) = delete;
    GodboltAgent(GodboltAgent&&) = delete;
    void operator=(GodboltAgent&&) = delete;
    ~GodboltAgent() override = default;

    void initialize(BackendInterface* backend);
    void compile(const CompileInfo& ci);
    const QString& getCompileOutput() const;
    const QString& getAsmContent() const;
    const AsmItemList &getAsmItems() const;
signals:
    void compiled();
private slots:
    void onCompileRequestFinished();
private:
    QNetworkAccessManager& m_nam;
    BackendInterface* m_backend;
    QString m_compileOutput;
    QString m_asmContent;
    AsmItemList m_asmItems;
};

#endif // GODBOLTAGENT_H
