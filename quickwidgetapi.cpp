#include "stdafx.h"
#include "quickwidgetapi.h"

QuickWidgetAPI::QuickWidgetAPI(QObject *parent)
    : QObject(parent)
    , m_binary(false)
    , m_binaryEnabled(false)
    , m_labels(true)
    , m_labelsEnabled(false)
    , m_trim(true)
    , m_trimEnabled(false)
    , m_directives(true)
    , m_directivesEnabled(false)
    , m_intel(false)
    , m_intelEnabled(false)
    , m_commentOnly(false)
    , m_commentOnlyEnabled(false)
    , m_demangle(true)
    , m_demangleEnabled(false)

{

}

void QuickWidgetAPI::closeConfiguration()
{
    emit doCloseConfiguration();
}

bool QuickWidgetAPI::binary() const
{
    return m_binary;
}

void QuickWidgetAPI::setBinary(bool binary)
{
    if (m_binary == binary)
        return;
    m_binary = binary;
    emit binaryChanged();
    qDebug() << __FUNCTION__ << binary;
}

bool QuickWidgetAPI::labels() const
{
    return m_labels;
}

void QuickWidgetAPI::setLabels(bool labels)
{
    if (m_labels == labels)
        return;
    m_labels = labels;
    emit labelsChanged();
    qDebug() << __FUNCTION__ << labels;
}

bool QuickWidgetAPI::trim() const
{
    return m_trim;
}

void QuickWidgetAPI::setTrim(bool trim)
{
    if (m_trim == trim)
        return;
    m_trim = trim;
    emit trimChanged();
    qDebug() << __FUNCTION__ << trim;
}

bool QuickWidgetAPI::directives() const
{
    return m_directives;
}

void QuickWidgetAPI::setDirectives(bool directives)
{
    if (m_directives == directives)
        return;
    m_directives = directives;
    emit directivesChanged();
    qDebug() << __FUNCTION__ << directives;
}

bool QuickWidgetAPI::intel() const
{
    return m_intel;
}

void QuickWidgetAPI::setIntel(bool intel)
{
    if (m_intel == intel)
        return;
    m_intel = intel;
    emit intelChanged();
    qDebug() << __FUNCTION__ << intel;
}

bool QuickWidgetAPI::commentOnly() const
{
    return m_commentOnly;
}

void QuickWidgetAPI::setCommentOnly(bool commentOnly)
{
    if (m_commentOnly == commentOnly)
        return;
    m_commentOnly = commentOnly;
    emit commentOnlyChanged();
    qDebug() << __FUNCTION__ << commentOnly;
}

bool QuickWidgetAPI::binaryEnabled() const
{
    return m_binaryEnabled;
}

void QuickWidgetAPI::setBinaryEnabled(bool binaryEnabled)
{
    if (m_binaryEnabled == binaryEnabled)
        return;
    m_binaryEnabled = binaryEnabled;
    return binaryEnabledChanged();
}

bool QuickWidgetAPI::labelsEnabled() const
{
    return m_labelsEnabled;
}

void QuickWidgetAPI::setLabelsEnabled(bool labelsEnabled)
{
    if (m_labelsEnabled == labelsEnabled)
        return;
    m_labelsEnabled = labelsEnabled;
    return labelsEnabledChanged();
}

bool QuickWidgetAPI::trimEnabled() const
{
    return m_trimEnabled;
}

void QuickWidgetAPI::setTrimEnabled(bool trimEnabled)
{
    if (m_trimEnabled == trimEnabled)
        return;
    m_trimEnabled = trimEnabled;
    return trimEnabledChanged();
}

bool QuickWidgetAPI::directivesEnabled() const
{
    return m_directivesEnabled;
}

void QuickWidgetAPI::setDirectivesEnabled(bool directivesEnabled)
{
    if (m_directivesEnabled == directivesEnabled)
        return;
    m_directivesEnabled = directivesEnabled;
    emit directivesEnabledChanged();
}

bool QuickWidgetAPI::intelEnabled() const
{
    return m_intelEnabled;
}

void QuickWidgetAPI::setIntelEnabled(bool intelEnabled)
{
    if (m_intelEnabled == intelEnabled)
        return;
    m_intelEnabled = intelEnabled;
    emit intelEnabledChanged();
}

bool QuickWidgetAPI::commentOnlyEnabled() const
{
    return m_commentOnlyEnabled;
}

void QuickWidgetAPI::setCommentOnlyEnabled(bool commentOnlyEnabled)
{
    if (m_commentOnlyEnabled == commentOnlyEnabled)
        return;
    m_commentOnlyEnabled = commentOnlyEnabled;
    emit commentOnlyEnabledChanged();
}

bool QuickWidgetAPI::demangle() const
{
    return m_demangle;
}

void QuickWidgetAPI::setDemangle(bool demangle)
{
    if (m_demangle == demangle)
        return;
    m_demangle = demangle;
    emit demangleChanged();
    qDebug() << __FUNCTION__ << demangle;
}

bool QuickWidgetAPI::demangleEnabled() const
{
    return m_demangleEnabled;
}

void QuickWidgetAPI::setDemangleEnabled(bool demangleEnabled)
{
    if (m_demangleEnabled == demangleEnabled)
        return;
    m_demangleEnabled = demangleEnabled;
    emit demangleEnabledChanged();
}
