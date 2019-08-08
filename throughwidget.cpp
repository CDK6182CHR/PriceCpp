#include "throughwidget.h"
#include "util.h"

ThroughWidget::ThroughWidget(const PriceList *oldPrice, const PriceList *newPrice,
                             QWidget *parent):
    throughPrice(oldPrice,newPrice),QWidget(parent)
{
    QGroupBox *throughGroup,*firstGroup;
    QVBoxLayout* vlayout=new QVBoxLayout;
    QFormLayout* flayout=new QFormLayout;
    throughGroup=new QGroupBox("全程");
    throughRadios=new TriRadios("普客到底","普快到底","特快到底");
    flayout->addRow("通票等级",throughRadios);
    totalEdit=new QLineEdit;
    totalEdit->setValidator(new QIntValidator(1,6000,this));
    flayout->addRow("全程里程",totalEdit);
    throughGroup->setLayout(flayout);
    vlayout->addWidget(throughGroup);

    firstGroup=new QGroupBox("首程");
    flayout=new QFormLayout;
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
    flayout->addRow("首程里程",mileEdit);
    firstGroup->setLayout(flayout);
    vlayout->addWidget(firstGroup);

    resultEdit=new QLineEdit;
    resultEdit->setFocusPolicy(Qt::NoFocus);
    vlayout->addWidget(resultEdit);

    QHBoxLayout* hlayout=new QHBoxLayout;
    QPushButton *btnCal,*btnDetail;
    btnCal=new QPushButton("计算");
    connect(btnCal,&QPushButton::clicked,this,&ThroughWidget::calculate);
    hlayout->addWidget(btnCal);
    btnDetail=new QPushButton("详细");
    connect(btnDetail,&QPushButton::clicked,this,&ThroughWidget::detail);
    hlayout->addWidget(btnDetail);
    vlayout->addLayout(hlayout);
    setLayout(vlayout);
}

void ThroughWidget::calAllSeats()
{
    int totalMile=totalEdit->text().toInt();
    ThroughType throughType=(ThroughType)(throughRadios->currentState());
    ACType acType=(ACType)(acRadios->currentState());
    FastType fastType=(FastType)(fastRadios->currentState());
    DiscountType discountType=(DiscountType)(discountRadios->currentState());
    int mile=mileEdit->text().toInt();
    int endSeat=6;
    if(discountType==DiscountType::student){
        endSeat=3;
    }
    else if(mile>totalMile){
        QMessageBox::warning(this,"错误","首程里程不能超过全程里程！");
        return;
    }
    else if((int)throughType>(int)fastType){
        QMessageBox::information(this,"提示","通票违例：全程加快等级超过首程加快等级。计算结果仅供参考。");
    }
    QString text;
    SeatType seatType;
    for(int i=0;i<=endSeat;i++){
        seatType=comboIndexToType(i);
        throughPrice.setData(mile,acType,seatType,fastType,discountType,
                             totalMile,throughType);
        text+=throughPrice.resultString()+"\n";
    }

    QDialog* dialog=new QDialog(this);
    dialog->setWindowTitle("所有席别");
    QVBoxLayout* vlayout=new QVBoxLayout;
    QLabel* label=new QLabel(text);
    label->setWordWrap(true);
    vlayout->addWidget(label);
    QPushButton* btn=new QPushButton("关闭");
    connect(btn,&QPushButton::clicked,dialog,&QDialog::close);
    vlayout->addWidget(btn);
    dialog->setLayout(vlayout);
    dialog->show();
}

void ThroughWidget::addTableRow(int row, QString name, double number, QTableWidget *tw)
{
    QString numberStr;
    numberStr.sprintf("%.2lf",number);
    tw->setItem(row,0,new QTableWidgetItem(name));
    tw->setItem(row,1,new QTableWidgetItem(numberStr));
}

/*
 * 验证以下内容
 * （1）首程加快等级，提示违规
 * （2）首程里程不得超过全程里程
 * （3）学生票不发售软席
 */
void ThroughWidget::calculate()
{
    if(seatCombo->currentIndex()==7){
        calAllSeats();
        return;
    }
    int totalMile=totalEdit->text().toInt();
    ThroughType throughType=(ThroughType)(throughRadios->currentState());
    ACType acType=(ACType)(acRadios->currentState());
    FastType fastType=(FastType)(fastRadios->currentState());
    DiscountType discountType=(DiscountType)(discountRadios->currentState());
    SeatType seatType=comboIndexToType(seatCombo->currentIndex());
    int mile=mileEdit->text().toInt();
    if(discountType==DiscountType::student && seatType>=10){
        QMessageBox::warning(this,"错误","学生票不发售软席！");
        return;
    }
    else if(mile>totalMile){
        QMessageBox::warning(this,"错误","首程里程不能超过全程里程！");
        return;
    }
    else if((int)throughType>(int)fastType){
        QMessageBox::information(this,"提示","通票违例：全程加快等级超过首程加快等级。计算结果仅供参考。");
    }
    throughPrice.setData(mile,acType,seatType,fastType,discountType,totalMile,throughType);
    resultEdit->setText(throughPrice.resultString());
}

void ThroughWidget::detail()
{
    calculate();
    QDialog* dialog=new QDialog(this);
    dialog->setWindowTitle("详细票价");
    QVBoxLayout* vlayout=new QVBoxLayout;
    QTableWidget* tw=new QTableWidget;
    tw->setRowCount(9);
    tw->setColumnCount(2);
    tw->setEditTriggers(tw->NoEditTriggers);
    QStringList lst;
    lst<<"项目"<<"票价";
    tw->setHorizontalHeaderLabels(lst);
    addTableRow(0,"底票客票",throughPrice.throughSeatPrice(),tw);
    addTableRow(1,"底票加快票",throughPrice.throughFastPrice(),tw);
    addTableRow(2,"首程客票补价",throughPrice.seatAddPrice(),tw);
    addTableRow(3,"首程加快补价",throughPrice.fastAddPrice(),tw);
    addTableRow(4,"首程空调票",throughPrice.acPrice(),tw);
    addTableRow(5,"首程卧铺票",throughPrice.sleeperPrice(),tw);
    addTableRow(6,"底票合计",throughPrice.throughFastPrice()+
                throughPrice.throughSeatPrice(),tw);
    addTableRow(7,"补价合计",throughPrice.addPrice(),tw);
    addTableRow(8,"联合票价",throughPrice.unionPrice(),tw);
    vlayout->addWidget(tw);
    QPushButton* btn=new QPushButton("关闭");
    connect(btn,&QPushButton::clicked,dialog,&QDialog::close);
    vlayout->addWidget(btn);
//    tw->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
//    dialog->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    dialog->setLayout(vlayout);
    dialog->showMaximized();
}
