#ifndef BASEPRICE_H
#define BASEPRICE_H
#pragma once

enum ACType{
  NonAc=0,OldAc=1,NewAc=2,
};//非空，空调，新空调
enum SeatType{
    HardSeat=0,HardSleeperL,HardSleeperM,HardSleeperU,
    SoftSeat=10,SoftSleeperL,SoftSleeperU
};//席别。>=10表示软席，允许直接用这个来快速判定。
enum FastType{
    Normal=0,Fast=1,Special=2
};//加快等级，允许直接当成因子使用
enum DiscountType{
    full=0,child=1,student=2
};//优惠票类型，允许直接利用full=0。

class Price;
class PriceListWidget;
class BasePrice
{
    int minMile,maxMile; //上下界。均包含
    double hSeat,sSeat; //客票票价
    double fast,special;//加快票价
    double hSleeperL,hSleeperM,hSleeperU;//硬卧下中上
    double sSleeperL,sSleeperU;//软卧下上
    double ac;//空调票
public:
    BasePrice();
    BasePrice(int minmile,int maxmile,double hseat,double sseat,
              double fast_,double special_,double hsleeperu,
              double hsleeperm,double hsleeperl,double ssleeperu,
              double ssleeperl,double ac_);
    inline int minMileage()const{return minMile;}
    inline int maxMileage()const{return maxMile;}
    double seatPrice(SeatType type)const;//客票票价
    double sleeperPrice(SeatType type)const;//卧铺票价，非卧铺返回0
    double speedPrice(FastType type)const;//加快票价
    friend class Price;
    friend class PriceListWidget;
};

#endif // BASEPRICE_H
