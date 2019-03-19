#include "stdafx.h"
#include "codeinspectorapp.h"
#include "godboltagent.h"
#include "codeinspectortabwidget.h"
#include "outputwindow.h"
#include "codeinspectorpane.h"

CodeInspectorPane::CodeInspectorPane(QWidget *parent)
    : QWidget(parent)
    , m_backend(new GodboltAgent(ciApp->networkAccessManager(), this))
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
    
    QSplitter *splitter = new QSplitter(Qt::Vertical, this);
    mainLayout->addWidget(splitter);
    
    m_codeInspectorTabWidget = new CodeInspectorTabWidget(this);
    splitter->addWidget(m_codeInspectorTabWidget);
    
    QWidget *outputPane = new QWidget(this);
    
    QHBoxLayout* outputBarLayout = new QHBoxLayout();
    outputBarLayout->setContentsMargins(0,0,0,0);
    outputBarLayout->setSpacing(0);
    
    QVBoxLayout* outputPaneLayout = new QVBoxLayout();
    outputPaneLayout->setContentsMargins(0,0,0,0);
    outputPaneLayout->setSpacing(0);
    outputPane->setLayout(outputPaneLayout);
    
    m_btnToggleOutput = new QPushButton(QIcon(":/resource/image/errmsg.png"), tr("Output Window"));
    m_btnToggleOutput->setIconSize(QSize(32, 32));
    m_btnToggleOutput->setFlat(true);
    m_btnToggleOutput->setToolTip(tr("Toggle output window"));
    outputBarLayout->addWidget(m_btnToggleOutput);
    outputBarLayout->addStretch(1);

    outputPaneLayout->addLayout(outputBarLayout);

    m_output = new OutputWindow(this);
    m_output->initialize();
    outputPaneLayout->addWidget(m_output);
    
    splitter->addWidget(outputPane);
    
//    m_output->setVisible(false);
//    m_btnToggleOutput->setVisible(false);
    connect(m_btnToggleOutput, &QPushButton::clicked, [&]{m_output->setVisible(!m_output->isVisible());});
    
    m_backend->initialize(ciApp);
}
