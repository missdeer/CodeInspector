#ifndef CODEINSPECTORAPP_H
#define CODEINSPECTORAPP_H

#include <QObject>
#include <QNetworkAccessManager>
#include "backendinterface.hpp"
#include "language.hpp"
#include "compiler.hpp"
#include "godboltagent.h"

class CodeInspectorApp : public QObject, public BackendInterface
{
    Q_OBJECT
    Q_DISABLE_COPY(CodeInspectorApp)
public:
    explicit CodeInspectorApp(QObject *parent = nullptr);
    
    QNetworkAccessManager &networkAccessManager();
    
    void initialize();
    LanguageList &getLanguageList() override;
    CompilerListPtr getCompilerList(const QString& languageName) override;
    LibraryListPtr getLibraryList(const QString& languageName) override;
    CompilerPtr getCompiler(const QString& language, const QString& compiler) override;
    const QString& getExample(const QString& language) const override;
    const QString& getDefaultCompilerName(const QString& languageName) override;
    bool canCompile(const QString &language, const QString &compiler) override;
    const QString& getCompilerId(CompilerListPtr compilerList, const QString& name) override;
    QStringList getExampleList(const QString& languageName);
    QString getExampleContent(const QString &languageName, const QString &exampleName);
signals:
    void compilerListReady();
    void languageListReady();
    void configurationReady();
    
public slots:
    void switchLanguage(const QString& name);
    void onCompilerListRequestFinished();
    void onLanguageListRequestFinished();
    void onConfigurationRequestFinished();
    
private:
    QNetworkAccessManager m_nam;
    LanguageList m_languageList;
    QMap<QString, CompilerListPtr> m_compilerMap; // language name - compiler list
    QMap<QString, QString> m_defaultCompiler; // language id - compiler id
    QMap<QString, LibraryListPtr> m_libs; // language id - library list
    GodboltAgent* m_backend;
    
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
};

extern CodeInspectorApp *ciApp;

#endif // CODEINSPECTORAPP_H
