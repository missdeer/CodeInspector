#ifndef OUTPUTWINDOW_H
#define OUTPUTWINDOW_H

#include "ScintillaEdit.h"
#include "scintillaconfig.h"

class OutputWindow : public ScintillaEdit
{
    Q_OBJECT
public:
    explicit OutputWindow(QWidget *parent = 0);
    void initialize();
    void setContent(const QString& content);
    void setContent(const QByteArray& content);
private:
    ScintillaConfig m_sc;
};

#endif // OUTPUTWINDOW_H
