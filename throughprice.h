/*
 * 通票票价类。继承的部分作为首程信息，补充的是全程信息。
 * */
#ifndef THROUGHPRICE_H
#define THROUGHPRICE_H
#pragma once
#include "price.h"
#include "pricelist.h"

enum ThroughType{
    Puke=0,Pukuai=1,Tekuai=2
};//通票到底类型
class ThroughPrice : public Price
{
protected:
    int totalMile;
    ThroughType throughType;
    const BasePrice* totalBase;
public:
    ThroughPrice(const PriceList* oldList,const PriceList* newList);
    void setData(int mile, ACType actype, SeatType seattype,
                 FastType fasttype, DiscountType discounttype,
                 int totalmile,ThroughType throughtype);
    double unionPrice()const;
    double throughSeatPrice()const; //通票全程客票。皆已经考虑优惠票。
    double throughFastPrice()const; //通票全程加快票
    double seatAddPrice()const;  //首程客票差价
    double fastAddPrice()const;  //首程加快差价
    double addPrice()const;  //首程补价合计
    double directPrice()const;  //首程直达票价合计
    QString typeString()const;
    QString resultString()const;
};

#endif // THROUGHPRICE_H
