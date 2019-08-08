#include "directwidget.h"
#include "util.h"
#include <QDebug>
DirectWidget::DirectWidget(const PriceList *oldlist,
                           const PriceList *newlist, QWidget *parent):
    QWidget(parent),oldList(oldlist),newList(newlist),price(oldList,newList)
{
    setWindowTitle("直达票价计算");
    QVBoxLayout* vlayout=new QVBoxLayout;
    QFormLayout* flayout=new QFormLayout;
    acRadios=new TriRadios("非空调","空调","新空调");
    flayout->addRow("车型",acRadios);
    fastRadios=new TriRadios("普客","普快","特快");
    flayout->addRow("加快",fastRadios);
    discountRadios=new TriRadios("全","孩","学");
    flayout->addRow("票种",discountRadios);
    seatCombo=new QComboBox;
    QStringList lst;
    lst<<"硬座"<<"硬卧上"<<"硬卧中"<<"硬卧下"<<"软座"<<"软卧上"<<"软卧下"<<"所有席别";
    seatCombo->addItems(lst);
    flayout->addRow("席别",seatCombo);
    mileEdit=new QLineEdit;
    mileEdit->setValidator(new QIntValidator(1,6000,this));
    flayout->addRow("里程",mileEdit);
    vlayout->addLayout(flayout);

    resultEdit=new QLineEdit;
    resultEdit->setFocusPolicy(Qt::NoFocus);
    vlayout->addWidget(resultEdit);

    QPushButton *btnCal,*btnDetail;
    btnCal=new QPushButton("计算");
    btnDetail=new QPushButton("详细");
    connect(btnCal,&QPushButton::clicked,this,&DirectWidget::calculate);
    connect(btnDetail,&QPushButton::clicked,this,&DirectWidget::detail);
    QHBoxLayout *hlayout=new QHBoxLayout;
    hlayout->addWidget(btnCal);
    hlayout->addWidget(btnDetail);
    vlayout->addLayout(hlayout);

    setLayout(vlayout);
}

void DirectWidget::calAllSeats()
{
    ACType acType=(ACType)(acRadios->currentState());
    FastType fastType=(FastType)(fastRadios->currentState());
    SeatType seatType;
    DiscountType discountType=(DiscountType)(discountRadios->currentState());
    int mile=mileEdit->text().toInt();
    int maxSeatIndex=6;
    if(discountType==DiscountType::student){
        maxSeatIndex=3;
    }
    QString text;
    for(int i=0;i<=maxSeatIndex;i++){
        seatType=comboIndexToType(i);
        price.setData(mile,acType,seatType,fastType,discountType);
        text+=price.resultString()+'\n';
    }
    QDialog* dialog=new QDialog;
    dialog->setWindowTitle("所有席别票价");
    QVBoxLayout* layout=new QVBoxLayout;
    QLabel* label=new QLabel(text);
    label->setWordWrap(true);
    layout->addWidget(label);
    QPushButton* btn=new QPushButton("关闭");
    connect(btn,&QPushButton::clicked,dialog,&QDialog::close);
    layout->addWidget(btn);
    dialog->setLayout(layout);
    dialog->showMaximized();
}

void DirectWidget::addTableRow(int row, const QString &name, double num,
                               QTableWidget* tw)
{
    tw->setItem(row,0,new QTableWidgetItem(name));
    tw->setItem(row,1,new QTableWidgetItem(QString::number(num,'.',2)));
}

void DirectWidget::calculate()
{
    if(seatCombo->currentIndex()==7){
        calAllSeats();
        return;
    }
    ACType acType=(ACType)(acRadios->currentState());
    FastType fastType=(FastType)(fastRadios->currentState());
    DiscountType discountType=(DiscountType)(discountRadios->currentState());
    SeatType seatType=comboIndexToType(seatCombo->currentIndex());
    int mile=mileEdit->text().toInt();
    if(discountType==DiscountType::student && seatType>=10){
        QMessageBox::warning(this,"错误","学生票不发售软席！");
        return;
    }
    price.setData(mile,acType,seatType,fastType,discountType);
    QString priceStr;
    priceStr.sprintf(" %.2lf元",price.unionPrice());
    resultEdit->setText(
        price.typeString()+priceStr
                );
}

void DirectWidget::detail()
{
    if(seatCombo->currentIndex()==7){
        QMessageBox::warning(this,"错误","“计算所有席别”选项不支持显示详细票价！");
        return;
    }
    calculate();
    QDialog* dialog=new QDialog(this);
    QVBoxLayout* vlayout=new QVBoxLayout;
    dialog->setWindowTitle("详细票价");
    QTableWidget* tw=new QTableWidget;
    tw->setColumnCount(2);
//    tw->setColumnWidth(0,100);
//    tw->setColumnWidth(1,100);
    QStringList l;
    l<<"项目"<<"票价";
    tw->setHorizontalHeaderLabels(l);
    tw->setRowCount(5);
    tw->setEditTriggers(tw->NoEditTriggers);
    addTableRow(0,"客票",price.seatPrice(),tw);
    addTableRow(1,"加快票",price.speedPrice(),tw);
    addTableRow(2,"空调票",price.acPrice(),tw);
    addTableRow(3,"卧铺票",price.sleeperPrice(),tw);
    addTableRow(4,"合计",price.unionPrice(),tw);
    vlayout->addWidget(tw);
    QPushButton* btn=new QPushButton("关闭");
    connect(btn,&QPushButton::clicked,dialog,&QDialog::close);
    vlayout->addWidget(btn);
    dialog->setLayout(vlayout);
    dialog->showMaximized();
}
