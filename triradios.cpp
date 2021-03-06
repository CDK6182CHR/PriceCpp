#include "triradios.h"

//TriRadios::TriRadios(QWidget *parent) : QWidget(parent)
//{

//}

TriRadios::TriRadios(const QString &label0, const QString &label1, const QString &label2)
{
    radio0=new QRadioButton(label0);
    radio1=new QRadioButton(label1);
    radio2=new QRadioButton(label2);
    radio0->setChecked(true);
    addWidget(radio0);
    addWidget(radio1);
    addWidget(radio2);
    QButtonGroup* group=new QButtonGroup(this);
    group->addButton(radio0);
    group->addButton(radio1);
    group->addButton(radio2);
}

int TriRadios::currentState() const
{
    if(radio0->isChecked())
        return 0;
    else if(radio1->isChecked())
        return 1;
    return 2;
}

void TriRadios::setEnabled(bool on)
{
    QHBoxLayout::setEnabled(on);
    if(on){
        radio0->setEnabled(true);
        radio1->setEnabled(true);
        radio2->setEnabled(true);
    }else{
        radio0->setEnabled(false);
        radio1->setEnabled(false);
        radio2->setEnabled(false);
    }
}


