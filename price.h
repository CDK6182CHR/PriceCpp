/*
 *直达票价。将basePrice作为成员对象来处理。
 */
#ifndef PRICE_H
#define PRICE_H
#pragma once
#include "baseprice.h"
#include "pricelist.h"
#include <QString>

class Price
{
    int mile;
    const PriceList* oldList,*newList;//非空表和新空表。
    ACType acType;
    SeatType seatType;
    FastType fastType;
    DiscountType discountType;
    const BasePrice* basePrice;
public:
    Price();
    Price(const PriceList* oldlist,const PriceList* newlist);
    void setData(int mile,ACType actype,SeatType seattype,FastType fasttype,
                 DiscountType discounttype);
    //构造函数中来找basePrice
    double unionPrice()const;
    QString typeString()const;//类型说明文字
    QString resultString()const;
    double seatPrice()const;//客票票价
    double sleeperPrice()const;//卧铺票价，非卧铺返回0
    double speedPrice()const;//加快票价
    double acPrice()const;//空调票价

private:
    static double correctPrice(double price);
    QString acString()const;
    QString seatString()const;
    QString fastString()const;
    QString sleeperString()const;
    QString discountString()const;
};

#endif // PRICE_H
