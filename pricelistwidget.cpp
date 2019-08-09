#include "pricelistwidget.h"

PriceListWidget::PriceListWidget(const PriceList *oldlist, const PriceList *newlist,
                                 QWidget *parent):
    QWidget(parent),oldList(oldlist),newList(newlist)
{
    setWindowTitle("票价表查询");
    QVBoxLayout* vlayout=new QVBoxLayout;
    QFormLayout* flayout=new QFormLayout;
    QHBoxLayout* hlayout=new QHBoxLayout;
    minMileEdit=new QLineEdit;
    minMileEdit->setValidator(new QIntValidator(1,6000,this));
    QLabel* l=new QLabel("-");
    maxMileEdit=new QLineEdit;
    maxMileEdit->setValidator(new QIntValidator(1,6000,this));
    hlayout->addWidget(minMileEdit);
    hlayout->addWidget(l);
    hlayout->addWidget(maxMileEdit);
    flayout->addRow("里程范围",hlayout);
    checkNew=new QCheckBox("新空调");
    flayout->addRow("车型",checkNew);
    vlayout->addLayout(flayout);

    QPushButton* btn=new QPushButton("查询");
    connect(btn,&QPushButton::clicked,this,&PriceListWidget::resetTable);
    vlayout->addWidget(btn);

    tw=new QTableWidget;
    tw->setColumnCount(8);
    tw->setEditTriggers(tw->NoEditTriggers);
    tw->setSelectionMode(tw->NoSelection);
    QScroller::grabGesture(tw,QScroller::TouchGesture);
    QStringList lst;
    lst<<"里程"<<"硬座"<<"软座"<<"普快"<<"特快"<<"硬卧"<<"软卧"<<"空调";
    tw->setHorizontalHeaderLabels(lst);

    vlayout->addWidget(tw);
    setLayout(vlayout);
}

void PriceListWidget::setCell(QTableWidget *tw, int row, int col, double value0, double value1, double value2)
{
    QString txt;
    if(value2!=-1)
        txt.sprintf("%.0lf/%.0lf/%.0lf",value0,value1,value2);
    else if(value1!=-1)
        txt.sprintf("%.0lf/%.0lf",value0,value1);
    else
        txt.sprintf("%.1lf",value0);
    tw->setItem(row,col,new QTableWidgetItem(txt));
}


void PriceListWidget::resetTable()
{
    int minMile,maxMile;
    minMile=minMileEdit->text().toInt();
    maxMile=maxMileEdit->text().toInt();
    const PriceList* pList=checkNew->isChecked()?newList:oldList;
    if(minMile>maxMile){
        QMessageBox::warning(this,"错误","里程上限必须大于或等于下限！");
        return;
    }
    int a=pList->findMileIndex(minMile),b=pList->findMileIndex(maxMile);
    tw->setRowCount(b-a+1);
    int row;
    for(int i=a;i<=b;i++){
        row=i-a;
        const BasePrice& bp=pList->operator [](i);
        QString mileStr;
        mileStr.sprintf("%d-%d",bp.minMileage(),bp.maxMileage());
        tw->setItem(row,0,new QTableWidgetItem(mileStr));
        setCell(tw,row,1,bp.hSeat);
        setCell(tw,row,2,bp.sSeat);
        setCell(tw,row,3,bp.fast);
        setCell(tw,row,4,bp.special);
        setCell(tw,row,5,bp.hSleeperU,bp.hSleeperM,bp.hSleeperL);
        setCell(tw,row,6,bp.sSleeperU,bp.sSleeperL);
        setCell(tw,row,7,bp.ac);
    }
}
