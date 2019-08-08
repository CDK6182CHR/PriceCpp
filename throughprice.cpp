#include "throughprice.h"

ThroughPrice::ThroughPrice(const PriceList *oldlist, const PriceList *newlist):
    Price(oldlist,newlist)
{

}

void ThroughPrice::setData(int mile, ACType actype, SeatType seattype,
                           FastType fasttype, DiscountType discounttype,
                           int totalmile, ThroughType throughtype)
{
    totalMile=totalmile;throughType=throughtype;
    Price::setData(mile,actype,seattype,fasttype,discounttype);
    totalBase=oldList->findMile(totalMile);
}


double ThroughPrice::unionPrice() const
{
    return throughSeatPrice()+throughFastPrice()+addPrice();
}

double ThroughPrice::throughSeatPrice() const
{
    if (discountType==full)
        return totalBase->seatPrice(SeatType::HardSeat);
    else
        return correctPrice(totalBase->seatPrice(SeatType::HardSeat));
}

double ThroughPrice::throughFastPrice() const
{
    double fast=totalBase->speedPrice((FastType)(int)throughType);
    if(discountType!=full)
        fast=correctPrice(fast/2);
    return fast;
}

double ThroughPrice::seatAddPrice() const
{
    const BasePrice* bp=oldList->findMile(mile);
    double firstBasePrice=bp->seatPrice(SeatType::HardSeat);
    if(discountType!=full)
        firstBasePrice=correctPrice(firstBasePrice/2);
    return seatPrice()-firstBasePrice;
}

double ThroughPrice::fastAddPrice() const
{
    if((int)fastType<=(int)throughType)
        return 0;
    const BasePrice* bp=oldList->findMile(mile);
    double fastBase=bp->speedPrice((FastType)(int)throughType);
    if(discountType!=full)
        fastBase=correctPrice(fastBase/2);
    return speedPrice()-fastBase;
}

double ThroughPrice::addPrice() const
{
    return fastAddPrice()+seatAddPrice()+acPrice()+sleeperPrice();
}

double ThroughPrice::directPrice() const
{
    return Price::unionPrice();
}

QString ThroughPrice::typeString() const
{
    return Price::typeString()+"通票";
}

QString ThroughPrice::resultString() const
{
    QString numStr;
    numStr.sprintf(" %.2lf",unionPrice());
    return typeString()+numStr;
}
