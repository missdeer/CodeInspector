#include "stdafx.h"
#include <QQmlEngine>
#include <QQmlContext>
#include "qmldialog.h"
#include "ui_qmldialog.h"

QmlDialog::QmlDialog(QWidget *parent, QObject *api) :
    QDialog(parent),
    ui(new Ui::QmlDialog)
{
    ui->setupUi(this);
    ui->quickWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->quickWidget->setAttribute(Qt::WA_AlwaysStackOnTop, true);
    ui->quickWidget->setClearColor(Qt::transparent);
    ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->quickWidget->engine()->rootContext()->setContextProperty("api", api);
}

QmlDialog::~QmlDialog()
{
    delete ui;
}

void QmlDialog::loadQml(const QUrl &u)
{
    ui->quickWidget->setSource(u);
}
