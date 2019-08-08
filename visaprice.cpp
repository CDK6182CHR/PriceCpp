#include "visaprice.h"

VisaPrice::VisaPrice(const PriceList *oldlist, const PriceList *newlist):
    ThroughPrice(oldlist,newlist),useInputPrice(false)
{

}

void VisaPrice::setData(int mile, ThroughType throughtype, DiscountType discounttype,
                        ACType actype, FastType fasttype, SeatType seattype)
{
    useInputPrice=false;
    ThroughPrice::setData(mile,actype,seattype,fasttype,discounttype,mile,throughtype);
}

void VisaPrice::setData(int mile, ThroughType throughtype, DiscountType discounttype,
                        double inputedprice)
{
    //此种情况下，basePrice没有用
    useInputPrice=true;
    inputedPrice=inputedprice;
    this->mile=mile;
    throughType=throughtype;
    discountType=discounttype;
    totalBase=oldList->findMile(mile);
}

double VisaPrice::unionPrice() const
{
    if(useInputPrice)
        return unionPriceInput();
    else
        return unionPriceSeat();
}

double VisaPrice::unionPriceSeat() const
{
    return addPrice();
}

double VisaPrice::unionPriceInput() const
{
    return inputedPrice-throughFastPrice()-throughSeatPrice();
}

double VisaPrice::fullPrice() const
{
    return inputedPrice;
}

QString VisaPrice::typeString() const
{
    if(!useInputPrice)
        return Price::typeString()+"差价";
    else
        return "签证差价";
}

QString VisaPrice::resultString() const
{
    QString s;
    s.sprintf(" %.2lf",unionPrice());
    return typeString()+s;
}
