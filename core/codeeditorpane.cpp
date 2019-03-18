#include "stdafx.h"
#include "codeeditor.h"
#include "outputwindow.h"
#include "codeeditorpane.h"

CodeEditorPane::CodeEditorPane(QWidget *parent) 
    : QWidget(parent)
{    
    QVBoxLayout *editorPanelLayout = new QVBoxLayout();
    setLayout(editorPanelLayout);
    editorPanelLayout->setContentsMargins(0,0,0,0);
    editorPanelLayout->setSpacing(0);

    m_codeEditor = new CodeEditor(this);
    m_codeEditor->initialize();

    editorPanelLayout->addWidget(m_codeEditor);

    QHBoxLayout* outputBarLayout = new QHBoxLayout();
    outputBarLayout->setContentsMargins(0,0,0,0);
    outputBarLayout->setSpacing(0);

    m_btnToggleOutput = new QPushButton(QIcon(":/resource/image/errmsg.png"), "");
#if defined(Q_OS_ANDROID)
    m_btnToggleOutput->setIconSize(QSize(120, 120));
#else
    m_btnToggleOutput->setIconSize(QSize(32, 32));
#endif
    m_btnToggleOutput->setFlat(true);
    m_btnToggleOutput->setToolTip(tr("Toggle output window"));
    outputBarLayout->addWidget(m_btnToggleOutput);
    outputBarLayout->addStretch(1);

    editorPanelLayout->addLayout(outputBarLayout);

    m_output = new OutputWindow(this);
    m_output->initialize();
    editorPanelLayout->addWidget(m_output);
}
