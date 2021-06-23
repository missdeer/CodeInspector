#ifndef ASTPLUGIN_H
#define ASTPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "options.h"

class AstPlugin
    : public QObject
    , public OptionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.OptionInterface" FILE "ast.json")
    Q_INTERFACES(OptionInterface)
public:
    explicit AstPlugin(QObject *parent = nullptr);
    ~AstPlugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(const QString &compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject& docObj) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;
signals:

private:
    QStringList m_supportedCompilers;
};

#endif // ASTPLUGIN_H
