#include "stdafx.h"
#include "codeeditor.h"
#include "outputwindow.h"
#include "codeeditorpane.h"

CodeEditorPane::CodeEditorPane(QWidget *parent) 
    : QWidget(parent)
{    
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    
    QHBoxLayout* argumentsBarLayout = new QHBoxLayout();
    argumentsBarLayout->setContentsMargins(0,0,0,0);
    argumentsBarLayout->setSpacing(0);
    
    m_compilerArguments = new QLineEdit(this);
    argumentsBarLayout->addWidget(m_compilerArguments);
    mainLayout->addLayout(argumentsBarLayout);

    m_codeEditor = new CodeEditor(this);
    m_codeEditor->initialize();

    mainLayout->addWidget(m_codeEditor);

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

    mainLayout->addLayout(outputBarLayout);

    m_output = new OutputWindow(this);
    m_output->initialize();
    mainLayout->addWidget(m_output);
}
