/*
 * 签证差价界面
 * */
#ifndef VISAWIDGET_H
#define VISAWIDGET_H
#pragma once

#include <QtWidgets>
#include "util.h"
#include "visaprice.h"
#include "triradios.h"
#include "triradiosv.h"

class VisaWidget : public QWidget
{
    Q_OBJECT
    VisaPrice visaPrice;
    QLineEdit *inputPriceEdit,*mileEdit,*resultEdit;
    TriRadiosV *throughRadios;
    TriRadios *fastRadios,*acRadios,*discountRadios;
    QComboBox *seatCombo;
    QCheckBox *useInputCheck;
public:
    explicit VisaWidget(const PriceList* oldlist,const PriceList* newlist,
                        QWidget *parent = nullptr);
private:
    void calAllSeats();
    void addTableRow(int row,const QString& name,double number,QTableWidget* tw);
signals:

private slots:
    void useInputChanged(bool checked);
    void calculate();
    void detail();
};

#endif // VISAWIDGET_H
