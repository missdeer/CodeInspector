#ifndef CLANGTIDYPLUGIN_H
#define CLANGTIDYPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class ClangTidyPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "clangtidy.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit ClangTidyPlugin(QObject *parent = nullptr);
    ~ClangTidyPlugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(const QString &compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject &docObj, QString &stdOut, QString &stdErr) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

private:
    QStringList m_supportedCompilers;
};

#endif // CLANGTIDYPLUGIN_H
