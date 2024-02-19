#include <QAction>
#include <QMenu>

#include "codeinspectortabwidget.h"
#include "codeinspector.h"

CodeInspectorTabWidget::CodeInspectorTabWidget(QWidget *parent) : QTabWidget(parent), m_codeInspector(new CodeInspector(this))
{
    setTabPosition(East);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &CodeInspectorTabWidget::onCustomContextMenuRequested);

    m_codeInspector->initialize();
    addTab(m_codeInspector, QIcon(":/resource/image/tab/inspector.png"), tr("Inspector"));

    // TODO: create tabs from plugins
}

void CodeInspectorTabWidget::setCodeInspectorAsmItems(const AsmItemList &items, bool binary, QMap<int, intptr_t> &markerMap)
{
    Q_ASSERT(m_codeInspector);
    m_codeInspector->setAsmItems(items, binary, markerMap);
}

void CodeInspectorTabWidget::setCodeInspectorContent(const QString &content, bool binary)
{
    Q_ASSERT(m_codeInspector);
    m_codeInspector->setContent(content, binary);
}

void CodeInspectorTabWidget::onCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.exec(mapToGlobal(pos));
}

void CodeInspectorTabWidget::removePage(QWidget **ppWidget)
{
    if (!*ppWidget)
    {
        return;
    }

    for (int index = 0; index < count(); index++)
    {
        if (widget(index) == *ppWidget)
        {
            removeTab(index);
            delete *ppWidget;
            *ppWidget = nullptr;
            break;
        }
    }
}
