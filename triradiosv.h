/*
 * 三选一的radioButton组合。继承水平布局实现。
 * 三个对象的编号为0，1，2，由currentState函数返回当前状态值。
 * */
#ifndef TRIRADIOSV_H
#define TRIRADIOSV_H

#include <QObject>
#include <QtWidgets>

class TriRadiosV : public QVBoxLayout
{
    Q_OBJECT
    QRadioButton *radio0,*radio1,*radio2;
public:
//    explicit TriRadios(QWidget *parent = nullptr);
    explicit TriRadiosV(const QString& label0,const QString& label1,
                       const QString& label2);
    int currentState()const;
signals:

public slots:
};

#endif // TRIRADIOSV_H
