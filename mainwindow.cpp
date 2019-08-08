#include "mainwindow.h"
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QFile file1(QString(":/rsc/oldList.txt")),
            file2(QString(":/rsc/newList.txt"));
    readList(&oldList,file1);
    readList(&newList,file2);
    setWindowTitle(title);
    QVBoxLayout* vlayout=new QVBoxLayout;
    vlayout->addWidget(new QLabel(title));
    QPushButton* btnAbout=new QPushButton("关于");
    connect(btnAbout,&QPushButton::clicked,this,&MainWindow::about);
//    btnAbout->setMaximumWidth(200);
    vlayout->addWidget(btnAbout);
    tabWidget=new QTabWidget;
    directWidget=new DirectWidget(&oldList,&newList);
    tabWidget->addTab(directWidget,"直达票价");
    throughWidget=new ThroughWidget(&oldList,&newList);
    tabWidget->addTab(throughWidget,"通票票价");
    visaWidget=new VisaWidget(&oldList,&newList);
    tabWidget->addTab(visaWidget,"签证票价");

    vlayout->addWidget(tabWidget);
    QWidget* widget=new QWidget;
    widget->setLayout(vlayout);
    setCentralWidget(widget);
}

MainWindow::~MainWindow()
{

}

void MainWindow::readList(PriceList *list, QFile& file)
{
    file.open(QFile::ReadOnly|QFile::Text);
    if(!file.isOpen()){
        return;
    }
    BasePrice* pPrice;
    while(!file.atEnd()){
        QString line=file.readLine();
        QStringList splited=line.split(' ');
        if(splited.length()<11){
            qDebug()<<"unexpectedLength"<<splited.length()<<line<<endl;
            continue;
        }
        int minMile,maxMile;
        minMile=splited[0].toInt();
        maxMile=splited[1].toInt();
        double hSeat,sSeat;
        hSeat=splited[2].toDouble();
        sSeat=splited[3].toDouble();
        double fast,special;
        fast=splited[4].toDouble();
        special=fast*2;
        double ywU,ywM,ywL,rwU,rwL;
        ywU=splited[5].toDouble();
        ywM=splited[6].toDouble();
        ywL=splited[7].toDouble();
        rwU=splited[8].toDouble();
        rwL=splited[9].toDouble();
        double ac=splited[10].toDouble();
        pPrice=new BasePrice(minMile,maxMile,hSeat,sSeat,fast,special,ywU,ywM,ywL,
                        rwU,rwL,ac);
        list->addPrice(*pPrice);
    }
    file.close();
}

void MainWindow::about()
{
    QMessageBox::about(this,"关于",title+"\n六方会谈 马兴越 mxy0268@qq.com\n保留所有权利"
                                       "\n结果仅供参考。\n"
                                       "源代码：https://github.com/CDK6182CHR/PriceCpp");
}
