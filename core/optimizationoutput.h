#ifndef OPTIMIZATIONOUTPUT_H
#define OPTIMIZATIONOUTPUT_H

#include <QObject>
#include "ScintillaEdit.h"
#include "scintillaconfig.h"
#include "optimizationitem.hpp"

class OptimizationOutput : public ScintillaEdit
{
    Q_OBJECT
public:
    explicit OptimizationOutput(QWidget *parent = nullptr);
    void initialize();
    void setContent(const OptimizationItemList &content);
protected:
     QVariant inputMethodQuery(Qt::InputMethodQuery) const override;
private:
    ScintillaConfig m_sc;
    void setContent(const QString& content);
};

#endif // OPTIMIZATIONOUTPUT_H
