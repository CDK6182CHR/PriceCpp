#include "visawidget.h"

VisaWidget::VisaWidget(const PriceList *oldlist, const PriceList *newlist, QWidget *parent):
    QWidget(parent),visaPrice(oldlist,newlist)
{
    QVBoxLayout* vlayout=new QVBoxLayout;
    QFormLayout* flayout=new QFormLayout;
    throughRadios=new TriRadiosV("普客到底","普快到底","特快到底");
    flayout->addRow("原票等级",throughRadios);
    discountRadios=new TriRadios("全","孩","学");
    flayout->addRow("票种",discountRadios);
    mileEdit=new QLineEdit;
    mileEdit->setValidator(new QIntValidator(1,6000,this));
    flayout->addRow("里程",mileEdit);
    useInputCheck=new QCheckBox("直达票价");
    useInputCheck->setChecked(false);
    inputPriceEdit=new QLineEdit;
    inputPriceEdit->setValidator(new QDoubleValidator(0,100000,1));
    inputPriceEdit->setEnabled(false);
    connect(useInputCheck,&QCheckBox::toggled,this,&VisaWidget::useInputChanged);
    flayout->addRow(useInputCheck,inputPriceEdit);

    acRadios=new TriRadios("非空调","空调","新空调");
    flayout->addRow("车型",acRadios);
    fastRadios=new TriRadios("普客","普快","特快");
    flayout->addRow("加快",fastRadios);
    seatCombo=new QComboBox;
    QStringList lst;
    lst<<"硬座"<<"硬卧上"<<"硬卧中"<<"硬卧下"<<"软座"<<"软卧上"<<"软卧下"<<"所有席别";
    seatCombo->addItems(lst);
    flayout->addRow("席别",seatCombo);

    vlayout->addLayout(flayout);
    resultEdit=new QLineEdit;
    resultEdit->setFocusPolicy(Qt::NoFocus);
    vlayout->addWidget(resultEdit);

    QHBoxLayout* hlayout=new QHBoxLayout;
    QPushButton *btnCal,*btnDetail;
    btnCal=new QPushButton("计算");
    hlayout->addWidget(btnCal);
    connect(btnCal,&QPushButton::clicked,this,&VisaWidget::calculate);
    btnDetail=new QPushButton("详细");
    hlayout->addWidget(btnDetail);
    connect(btnDetail,&QPushButton::clicked,this,&VisaWidget::detail);
    vlayout->addLayout(hlayout);
    setLayout(vlayout);
}

void VisaWidget::calAllSeats()
{
    int mile=mileEdit->text().toInt();
    ThroughType throughType=(ThroughType)throughRadios->currentState();
    DiscountType discountType=(DiscountType)discountRadios->currentState();
    SeatType seatType;
    ACType acType=(ACType)acRadios->currentState();
    FastType fastType=(FastType)fastRadios->currentState();
    int endIndex=discountType==student?3:6;
    QString text;
    for(int i=0;i<=endIndex;i++){
        seatType=comboIndexToType(i);
        visaPrice.setData(mile,throughType,discountType,acType,fastType,seatType);
        text+=visaPrice.resultString()+'\n';
    }
    QDialog* dialog=new QDialog(this);
    dialog->setWindowTitle("全部席别");
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

void VisaWidget::addTableRow(int row, const QString &name, double number, QTableWidget *tw)
{
    QString s;
    s.sprintf("%.2lf",number);
    tw->setItem(row,0,new QTableWidgetItem(name));
    tw->setItem(row,1,new QTableWidgetItem(s));
}

void VisaWidget::useInputChanged(bool checked)
{
    if(checked){
        inputPriceEdit->setEnabled(true);
        acRadios->setEnabled(false);
        fastRadios->setEnabled(false);
        seatCombo->setEnabled(false);
    }
    else{
        inputPriceEdit->setEnabled(false);
        acRadios->setEnabled(true);
        fastRadios->setEnabled(true);
        seatCombo->setEnabled(true);
    }
}

void VisaWidget::calculate()
{
    int mile=mileEdit->text().toInt();
    ThroughType throughType=(ThroughType)throughRadios->currentState();
    DiscountType discountType=(DiscountType)discountRadios->currentState();
    if(useInputCheck->isChecked()){
        double inputed=inputPriceEdit->text().toDouble();
        visaPrice.setData(mile,throughType,discountType,inputed);
    }//if inputIsUsed
    else{
        if(seatCombo->currentIndex()==7){
            calAllSeats();
            return;
        }
        SeatType seatType=comboIndexToType(seatCombo->currentIndex());
        ACType acType=(ACType)acRadios->currentState();
        FastType fastType=(FastType)fastRadios->currentState();
        if(discountType==student && seatType>=10){
            QMessageBox::warning(this,"错误","学生票不发售软席！");
            return;
        }
        else if((int)throughType>(int)fastType){
            QMessageBox::information(this,"提示","通票违例：加快等级小于底票加快等级，底票多出的"
                                               "加快票将被忽略，结果仅供参考。");
        }
        visaPrice.setData(mile,throughType,discountType,acType,fastType,seatType);
    }//!inputIsUsed
    resultEdit->setText(visaPrice.resultString());
}

void VisaWidget::detail()
{
    QDialog* dialog=new QDialog(this);
    dialog->setWindowTitle("详细票价");
    QVBoxLayout* vlayout=new QVBoxLayout;
    QTableWidget* tw=new QTableWidget;
    tw->setColumnCount(2);
    tw->setEditTriggers(tw->NoEditTriggers);
    QStringList lst;
    lst<<"项目"<<"票价";
    tw->setHorizontalHeaderLabels(lst);
    if(visaPrice.inputIsUsed()){
        tw->setRowCount(5);
        addTableRow(0,"底票客票",visaPrice.throughSeatPrice(),tw);
        addTableRow(1,"底票加快票",visaPrice.throughFastPrice(),tw);
        addTableRow(2,"底票合计",visaPrice.throughSeatPrice()+
                    visaPrice.throughFastPrice(),tw);
        addTableRow(3,"直达票价",visaPrice.fullPrice(),tw);
        addTableRow(4,"签证差价",visaPrice.unionPriceInput(),tw);
    }//inputIsUsed
    else{
        if(seatCombo->currentIndex()==7){
            QMessageBox::warning(this,"错误","“计算所有席别”选项下不支持显示详细票价！");
            return;
        }
        tw->setRowCount(9);
        addTableRow(0,"底票客票",visaPrice.throughSeatPrice(),tw);
        addTableRow(1,"底票加快票",visaPrice.throughFastPrice(),tw);
        addTableRow(2,"客票补价",visaPrice.seatAddPrice(),tw);
        addTableRow(3,"加快票补价",visaPrice.fastAddPrice(),tw);
        addTableRow(4,"空调票",visaPrice.acPrice(),tw);
        addTableRow(5,"卧铺票",visaPrice.sleeperPrice(),tw);
        addTableRow(6,"底票合计",visaPrice.throughSeatPrice()+
                    visaPrice.throughFastPrice(),tw);
        addTableRow(7,"直达票价",visaPrice.directPrice(),tw);
        addTableRow(8,"签证差价",visaPrice.unionPrice(),tw);
    }//!inputIsUsed
    vlayout->addWidget(tw);
    QPushButton* btn=new QPushButton("关闭");
    vlayout->addWidget(btn);
    connect(btn,&QPushButton::clicked,dialog,&QDialog::close);
    dialog->setLayout(vlayout);
    dialog->showMaximized();
}
