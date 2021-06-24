#ifndef OPTIMIZATIONOUTPUT_H
#define OPTIMIZATIONOUTPUT_H

#include <QTableWidget>

#include "optimizationitem.hpp"

class OptimizationOutput : public QTableWidget
{
    Q_OBJECT
public:
    explicit OptimizationOutput(QWidget *parent = nullptr);
    void initialize();
    void setContent(const OptimizationItemList &content);

protected:
private:
};

#endif // OPTIMIZATIONOUTPUT_H
