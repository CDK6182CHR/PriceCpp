/*
 *直达票价计算界面。
 */
#ifndef DIRECTWIDGET_H
#define DIRECTWIDGET_H
#pragma once
#include <QObject>
#include <QtWidgets>
#include "price.h"
#include "triradios.h"
class DirectWidget : public QWidget
{
    Q_OBJECT
    const PriceList *oldList,*newList;
    Price price;
    TriRadios *acRadios,*fastRadios,*discountRadios;
    QComboBox *seatCombo;
    QLineEdit *resultEdit,*mileEdit;
public:
    explicit DirectWidget(const PriceList*oldlist,const PriceList* newList,
                          QWidget *parent = nullptr);
private:
    void calAllSeats();//计算所有席别
    void addTableRow(int row,const QString& name,double num,QTableWidget* tw);
signals:

public slots:
    void calculate();
    void detail();
    void calMile();
};

#endif // DIRECTWIDGET_H
