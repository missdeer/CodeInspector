#ifndef OPTIONS_INTERFACE_H
#define OPTIONS_INTERFACE_H

#include <QtPlugin>

class OptionInterface
{
    virtual ~OptionInterface() = default;
    virtual bool isSupported() const = 0;
};

QT_BEGIN_NAMESPACE
#define OptionInterface_iid "com.ismisv.codeinspector.OptionInterface/1.0"

Q_DECLARE_INTERFACE(OptionInterface, OptionInterface_iid)
QT_END_NAMESPACE

#endif