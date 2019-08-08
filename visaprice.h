/*
 * 签证票价类。继承通票类，通票里程和首程里程都设置为签证的里程。
 * 可选择由席别计算还是由原价计算差价。
 * 需重新实现unionPrice，其他基本上不用动。
 * 默认情况票价由席别计算。
 * */
#ifndef VISAPRICE_H
#define VISAPRICE_H
#pragma once
#include "throughprice.h"

class VisaPrice : public ThroughPrice
{
    bool useInputPrice;
    double inputedPrice;
public:
    VisaPrice(const PriceList* oldlist,const PriceList* newlist);
    void setData(int mile,ThroughType throughtype,DiscountType discounttype,
                 ACType actype,FastType fasttype,SeatType seattype);
    void setData(int mile,ThroughType throughtype,DiscountType discounttype,
                 double inputedprice);
    double unionPrice()const;
    double unionPriceSeat()const; //由座别计算
    double unionPriceInput()const; //由输入票价计算
    double fullPrice()const;//输入的总价
    QString typeString()const;
    QString resultString()const;
    inline bool inputIsUsed()const{return useInputPrice;}
};

#endif // VISAPRICE_H
