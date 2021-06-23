#include "x86to6502plugin.h"

X86To6502Plugin::X86To6502Plugin(QObject *parent) : QObject(parent)
{
    
}

bool X86To6502Plugin::isCompilerSupported(int compilerId) const
{
    return false;
}

bool X86To6502Plugin::isSessionEnabled() const
{
    return false;
}

bool X86To6502Plugin::hasResult() const
{
    return false;
}

QWidget *X86To6502Plugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
