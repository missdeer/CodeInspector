#include "stdafx.h"
#include "codeinspectortabwidget.h"
#include "outputwindow.h"
#include "codeinspectorpane.h"

CodeInspectorPane::CodeInspectorPane(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    
    QHBoxLayout* topBarLayout = new QHBoxLayout();
    topBarLayout->setContentsMargins(0,0,0,0);
    topBarLayout->setSpacing(0);
    
    m_compilerList = new QComboBox(this);
    topBarLayout->addWidget(m_compilerList);
    
    m_compilerArguments = new QLineEdit(this);
    topBarLayout->addWidget(m_compilerArguments);
    
    mainLayout->addLayout(topBarLayout);
    
    m_codeInspectorTabWidget = new CodeInspectorTabWidget(this);
    mainLayout->addWidget(m_codeInspectorTabWidget);
    
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
    m_output->setVisible(false);
    m_btnToggleOutput->setVisible(false);
}
