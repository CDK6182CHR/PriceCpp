#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "pricelist.h"
#include "directwidget.h"
#include "throughwidget.h"
#include "visawidget.h"
#include "pricelistwidget.h"
#include "milewidget.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
    PriceList oldList,newList;
    DirectWidget *directWidget;
    ThroughWidget *throughWidget;
    VisaWidget *visaWidget;
    MileWidget *mileWidget;
    PriceListWidget *priceListWidget;
    QTabWidget* tabWidget;
    QString title="票价计算器 V4.2.1 20201117";
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void readList(PriceList* list,QFile& file);
private slots:
    void about();
};

#endif // MAINWINDOW_H
