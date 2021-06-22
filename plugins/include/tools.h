#ifndef TOOLS_INTERFACE_H
#define TOOLS_INTERFACE_H

#include <QtPlugin>

class ToolInterface
{
    virtual ~ToolInterface() = default;
    virtual bool isSupported() const = 0;
};

QT_BEGIN_NAMESPACE
#define ToolInterface_iid "com.ismisv.codeinspector.ToolInterface/1.0"

Q_DECLARE_INTERFACE(ToolInterface, ToolInterface_iid)
QT_END_NAMESPACE

#endif
