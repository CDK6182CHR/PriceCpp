#include"util.h"

/*
 * 将comboBox的序号转换为SeatType枚举值。顺序是：
 * lst<<"硬座"<<"硬卧上"<<"硬卧中"<<"硬卧下"<<"软座"<<"软卧上"<<"软卧下"<<"所有席别";
 * 要求保证输入的index在[0,6]内（不允许是“所有席别”）。
 * */
SeatType comboIndexToType(int index)
{
    SeatType seatType;
    switch(index){
    case 0:seatType=HardSeat;break;
    case 1:seatType=HardSleeperU;break;
    case 2:seatType=HardSleeperM;break;
    case 3:seatType=HardSleeperL;break;
    case 4:seatType=SoftSeat;break;
    case 5:seatType=SoftSleeperU;break;
    case 6:seatType=SoftSleeperL;break;
    }
    return seatType;
}
