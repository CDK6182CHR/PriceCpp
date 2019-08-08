#include "price.h"

Price::Price()
{

}

Price::Price(const PriceList *oldlist, const PriceList* newlist):
    oldList(oldlist),newList(newlist)
{

}

void Price::setData(int mile_, ACType actype, SeatType seattype,
                    FastType fasttype, DiscountType discounttype)
{
    mile=mile_;
    acType=actype;
    seatType=seattype;
    fastType=fasttype;
    discountType=discounttype;
    if(acType==NewAc)
        basePrice=newList->findMile(mile);
    else
        basePrice=oldList->findMile(mile);
}

double Price::unionPrice() const
{
    if(basePrice==nullptr)
        return -1;
    else if(seatType>=10 && discountType==student)
        return -1;//学生票不发售软席，其他与孩票一样
    double total=basePrice->seatPrice(seatType)+
            basePrice->speedPrice(fastType);
    if(acType!=NonAc)
        total+=basePrice->ac;
    if(discountType!=full)
        total=correctPrice(total/2);
    total+=basePrice->sleeperPrice(seatType);
    return total;
}

QString Price::typeString() const
{
    return acString()+seatString()+fastString()+
            sleeperString()+discountString();
}

QString Price::resultString() const
{
    QString priceStr;
    priceStr.sprintf(" %.2lf元",unionPrice());
    return typeString()+priceStr;
}

double Price::seatPrice() const
{
    if(discountType==full)
        return basePrice->seatPrice(seatType);
    else
        return correctPrice(basePrice->seatPrice(seatType)/2);
}

double Price::sleeperPrice() const
{
    return basePrice->sleeperPrice(seatType);
}

double Price::speedPrice() const
{
    if(discountType==full)
        return basePrice->speedPrice(fastType);
    else
        return correctPrice(basePrice->speedPrice(fastType)/2);
}

double Price::acPrice() const
{
    if(acType==NonAc)
        return 0;
    else if(discountType!=full)
        return correctPrice(basePrice->ac/2);
    return basePrice->ac;
}

double Price::correctPrice(double price)
{
    //尾数只可能是0,0.25，0.5，0.75.0.25变为0，0.75变为0.5.
    //为防止浮点型误差，尾巴上加一点点数据。
    int iprice=price;
    if(price-iprice<=0.26)
        return iprice;
    else if(price-iprice<=0.76)
        return iprice+0.5;
    else
        return iprice+1.0;
}

QString Price::acString()const
{
    if(acType==NonAc)
        return "";
    else if(acType==NewAc)
        return "新空调";
    else
        return "空调";
}

QString Price::seatString()const
{
    if(seatType>=10)
        return "软座";
    else
        return "硬座";
}

QString Price::fastString()const
{
    if(fastType==Normal)
        return "";
    else if(fastType==Fast)
        return "快";
    else
        return "特快";
}

QString Price::sleeperString()const
{
    switch(seatType){
    case SeatType::HardSeat:
    case SeatType::SoftSeat:return "";
    case SeatType::SoftSleeperL:
    case SeatType::HardSleeperL:return "卧(下)";
    case SeatType::HardSleeperM:return "卧(中)";
    case SeatType::HardSleeperU:
    case SeatType::SoftSleeperU:return "卧(上)";
    default:return "错误的类型";
    }
}

QString Price::discountString()const
{
    switch(discountType){
    case DiscountType::full:return "(全)";
    case DiscountType::student:return "(学)";
    case DiscountType::child:return "(孩)";
    }
    return "错误";
}
