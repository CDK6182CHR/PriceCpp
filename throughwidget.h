#ifndef THROUGHWIDGET_H
#define THROUGHWIDGET_H
#pragma once
#include <QtWidgets>
#include "throughprice.h"
#include "triradios.h"
#include "triradiosv.h"

class ThroughWidget : public QWidget
{
    Q_OBJECT
    ThroughPrice throughPrice;
    QLineEdit *resultEdit,*totalEdit,*mileEdit;
    QComboBox *seatCombo;
    TriRadios *acRadios,*fastRadios,*discountRadios;
    TriRadiosV *throughRadios;
public:
    explicit ThroughWidget(const PriceList* oldPrice,const PriceList* newPrice,
                           QWidget *parent = nullptr);
private:
    void calAllSeats();
    void addTableRow(int row,QString name,double number,QTableWidget* tw);
signals:

private slots:
    void calculate();
    void detail();
};

#endif // THROUGHWIDGET_H
