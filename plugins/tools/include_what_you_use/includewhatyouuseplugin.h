#ifndef INCLUDEWHATYOUUSEPLUGIN_H
#define INCLUDEWHATYOUUSEPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class IncludeWhatYouUsePlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "include_what_you_use.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit IncludeWhatYouUsePlugin(QObject *parent = nullptr);
    ~IncludeWhatYouUsePlugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(const QString &compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject& docObj) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

private:
    QStringList m_supportedCompilers;
};

#endif // INCLUDEWHATYOUUSEPLUGIN_H
