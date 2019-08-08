#include "triradiosv.h"

//TriRadios::TriRadios(QWidget *parent) : QWidget(parent)
//{

//}

TriRadiosV::TriRadiosV(const QString &label0, const QString &label1, const QString &label2)
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

int TriRadiosV::currentState() const
{
    if(radio0->isChecked())
        return 0;
    else if(radio1->isChecked())
        return 1;
    return 2;
}


