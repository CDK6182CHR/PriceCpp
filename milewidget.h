#ifndef MILEWIDGET_H
#define MILEWIDGET_H
#include "crnet.h"
#include <QtWidgets>

class MileWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MileWidget(QWidget *parent = nullptr);

signals:
    void explains();
    void mileCalculated(QString);
public slots:
private slots:
    void calculate();
private:
    CRNet& crNet;//默认构造
    void initUI();
    QLineEdit *startEdit,*endEdit,*mileEdit;
    QTextBrowser *pathText;
};

#endif // MILEWIDGET_H
