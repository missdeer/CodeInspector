#include <QHeaderView>

#include "optimizationoutput.h"

OptimizationOutput::OptimizationOutput(QWidget *parent) : QTableWidget(parent) {}

void OptimizationOutput::initialize()
{
    setColumnCount(5);
    setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Pass")));
    setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Name")));
    setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Type")));
    setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Function")));
    setHorizontalHeaderItem(4, new QTableWidgetItem(tr("Conclusion")));
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
}

void OptimizationOutput::setContent(const OptimizationItemList &content)
{
    clear();
    initialize();
    setRowCount(content.size());
    int index = 0;
    for (const auto &oi : content)
    {
        auto *pass = new QTableWidgetItem(oi->pass);
        setItem(index, 0, pass);
        auto *name = new QTableWidgetItem(oi->name);
        setItem(index, 1, name);
        auto *type = new QTableWidgetItem(oi->type);
        setItem(index, 2, type);
        auto *function = new QTableWidgetItem(oi->function);
        setItem(index, 3, function);
        auto *conclusion = new QTableWidgetItem(oi->display);
        setItem(index, 4, conclusion);
        index++;
    }
}
