#include "pricelist.h"
#include <QDebug>
PriceList::PriceList():list()
{

}

void PriceList::addPrice(const BasePrice &price)
{
    list.append(price);
}

const BasePrice *PriceList::findMile(int mile) const
/*二分法查找对应的里程。初期不实现6000以上的处理。*/
{
    if(mile>6000)
        return nullptr;
    int maxmile=list.length()-1,minmile=0;//在[min,max]范围进行二分查找。
    int middle1;
    while(maxmile-minmile>0){
        middle1=(maxmile+minmile)/2;
        if(mile<list[middle1].minMileage())
            maxmile=middle1-1;
        else if(mile>list[middle1].maxMileage())
            minmile=middle1+1;
        else{
            return &list[middle1];
        }
    }
    return &list[minmile];
}
