#ifndef PRICELISTWIDGET_H
#define PRICELISTWIDGET_H
#pragma once

#include <QtWidgets>
#include "pricelist.h"

class PriceListWidget : public QWidget
{
    Q_OBJECT
    const PriceList *oldList,*newList;
    QLineEdit *minMileEdit,*maxMileEdit;
    QTableWidget* tw;
    QCheckBox* checkNew;
public:
    explicit PriceListWidget(const PriceList* oldList,const PriceList* newList,
                             QWidget *parent = nullptr);
private:
    void setCell(QTableWidget* tw,int row,int col,double value0,double value1=-1,double value2=-1);
signals:

private slots:
    void resetTable();
};

#endif // PRICELISTWIDGET_H
