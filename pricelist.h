#ifndef PRICELIST_H
#define PRICELIST_H
#pragma once
#include "baseprice.h"
#include<QList>

class PriceList
{
    QList<BasePrice> list;
public:
    PriceList();
    void addPrice(const BasePrice& price);//按顺序添加
    const BasePrice* findMile(int mile)const;//不支持超过6000km
    int findMileIndex(int mile)const;//查找里程对应的序号
    const BasePrice& operator[](int i)const;
    BasePrice& operator [](int i);
};

#endif // PRICELIST_H
