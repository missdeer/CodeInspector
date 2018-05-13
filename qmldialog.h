#ifndef QMLDIALOG_H
#define QMLDIALOG_H

#include <QDialog>

namespace Ui {
class QmlDialog;
}

class QmlDialog : public QDialog
{
    Q_OBJECT

public:
    QmlDialog(QWidget *parent, QObject* api);
    void loadQml(const QUrl& u);
    ~QmlDialog();

private:
    Ui::QmlDialog *ui;
};

#endif // QMLDIALOG_H
