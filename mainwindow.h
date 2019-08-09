#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "pricelist.h"
#include "directwidget.h"
#include "throughwidget.h"
#include "visawidget.h"
#include "pricelistwidget.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
    PriceList oldList,newList;
    DirectWidget *directWidget;
    ThroughWidget *throughWidget;
    VisaWidget *visaWidget;
    PriceListWidget *priceListWidget;
    QTabWidget* tabWidget;
    QString title="票价计算器 V4.1.1 20190809";
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void readList(PriceList* list,QFile& file);
private slots:
    void about();
};

#endif // MAINWINDOW_H
