#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "pricelist.h"
#include "directwidget.h"
#include "throughwidget.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
    PriceList oldList,newList;
    DirectWidget *directWidget;
    ThroughWidget *throughWidget;
    QTabWidget* tabWidget;
    QString title="票价计算器 V4.0.0 Dev2 20190808";
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void readList(PriceList* list,QFile& file);
private slots:
    void about();
};

#endif // MAINWINDOW_H
