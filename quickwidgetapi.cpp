#include "stdafx.h"
#include "quickwidgetapi.h"

QuickWidgetAPI::QuickWidgetAPI(QObject *parent) : QObject(parent)
{

}

bool QuickWidgetAPI::binary() const
{
    return m_binary;
}

void QuickWidgetAPI::setBinary(bool binary)
{
    m_binary = binary;
    emit binaryChanged();
}

bool QuickWidgetAPI::labels() const
{
    return m_labels;
}

void QuickWidgetAPI::setLabels(bool labels)
{
    m_labels = labels;
    emit labelsChanged();
}

bool QuickWidgetAPI::trim() const
{
    return m_trim;
}

void QuickWidgetAPI::setTrim(bool trim)
{
    m_trim = trim;
    emit trimChanged();
}

bool QuickWidgetAPI::directives() const
{
    return m_directives;
}

void QuickWidgetAPI::setDirectives(bool directives)
{
    m_directives = directives;
    emit directivesChanged();
}

bool QuickWidgetAPI::intel() const
{
    return m_intel;
}

void QuickWidgetAPI::setIntel(bool intel)
{
    m_intel = intel;
    emit intelChanged();
}

bool QuickWidgetAPI::commentOnly() const
{
    return m_commentOnly;
}

void QuickWidgetAPI::setCommentOnly(bool commentOnly)
{
    m_commentOnly = commentOnly;
    emit commentOnlyChanged();
}

bool QuickWidgetAPI::binaryEnabled() const
{
    return m_binaryEnabled;
}

void QuickWidgetAPI::setBinaryEnabled(bool binaryEnabled)
{
    m_binaryEnabled = binaryEnabled;
    return binaryEnabledChanged();
}

bool QuickWidgetAPI::labelsEnabled() const
{
    return m_labelsEnabled;
}

void QuickWidgetAPI::setLabelsEnabled(bool labelsEnabled)
{
    m_labelsEnabled = labelsEnabled;
    return labelsEnabledChanged();
}

bool QuickWidgetAPI::trimEnabled() const
{
    return m_trimEnabled;
}

void QuickWidgetAPI::setTrimEnabled(bool trimEnabled)
{
    m_trimEnabled = trimEnabled;
    return trimEnabledChanged();
}

bool QuickWidgetAPI::directivesEnabled() const
{
    return m_directivesEnabled;
}

void QuickWidgetAPI::setDirectivesEnabled(bool directivesEnabled)
{
    m_directivesEnabled = directivesEnabled;
    emit directivesEnabledChanged();
}

bool QuickWidgetAPI::intelEnabled() const
{
    return m_intelEnabled;
}

void QuickWidgetAPI::setIntelEnabled(bool intelEnabled)
{
    m_intelEnabled = intelEnabled;
    emit intelEnabledChanged();
}

bool QuickWidgetAPI::commentOnlyEnabled() const
{
    return m_commentOnlyEnabled;
}

void QuickWidgetAPI::setCommentOnlyEnabled(bool commentOnlyEnabled)
{
    m_commentOnlyEnabled = commentOnlyEnabled;
    emit commentOnlyEnabledChanged();
}

bool QuickWidgetAPI::demangle() const
{
    return m_demangle;
}

void QuickWidgetAPI::setDemangle(bool demangle)
{
    m_demangle = demangle;
    emit demangleChanged();
}

bool QuickWidgetAPI::demangleEnabled() const
{
    return m_demangleEnabled;
}

void QuickWidgetAPI::setDemangleEnabled(bool demangleEnabled)
{
    m_demangleEnabled = demangleEnabled;
    emit demangleEnabledChanged();
}
