#ifndef CLANGFORMATPLUGIN_H
#define CLANGFORMATPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class ClangFormatPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "clang_format.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit ClangFormatPlugin(QObject *parent = nullptr);
    ~ClangFormatPlugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(const QString &compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject& docObj) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

private:
    QStringList m_supportedCompilers;
};

#endif // CLANGFORMATPLUGIN_H
