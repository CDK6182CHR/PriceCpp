#include "baseprice.h"

BasePrice::BasePrice()
{

}

BasePrice::BasePrice(int minmile,int maxmile,double hseat,double sseat,
          double fast_,double special_,double hsleeperu,
          double hsleeperm,double hsleeperl,double ssleeperu,
          double ssleeperl,double ac_):
minMile(minmile),maxMile(maxmile),hSeat(hseat),sSeat(sseat),fast(fast_),
  special(special_),hSleeperL(hsleeperl),hSleeperM(hsleeperm),
  hSleeperU(hsleeperu),sSleeperL(ssleeperl),sSleeperU(ssleeperu),ac(ac_)
{

}

double BasePrice::seatPrice(SeatType type) const
{
    if(type>=10)
        return sSeat;
    else
        return hSeat;
}

double BasePrice::sleeperPrice(SeatType type) const
{
    switch(type){
    case SeatType::HardSeat:
    case SeatType::SoftSeat:return 0;
    case SeatType::HardSleeperL:return hSleeperL;
    case SeatType::HardSleeperM:return hSleeperM;
    case SeatType::HardSleeperU:return hSleeperU;
    case SeatType::SoftSleeperU:return sSleeperU;
    case SeatType::SoftSleeperL:return sSleeperL;
    default:return -1;
    }
}

double BasePrice::speedPrice(FastType type) const
{
    switch(type){
    case FastType::Normal:return 0;
    case FastType::Fast:return fast;
    case FastType::Special:return special;
    default:return -1;
    }
}
