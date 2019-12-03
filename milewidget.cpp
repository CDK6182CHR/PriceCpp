#include "milewidget.h"

MileWidget::MileWidget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("里程计算");
    initUI();
}

void MileWidget::initUI()
{
    QVBoxLayout* vlayout=new QVBoxLayout;
    QFormLayout* flayout=new QFormLayout;
    startEdit=new QLineEdit;
    flayout->addRow("起始站",startEdit);
    endEdit=new QLineEdit;
    flayout->addRow("到达站",endEdit);
    vlayout->addLayout(flayout);
    flayout=new QFormLayout;
    QPushButton* btn=new QPushButton("计算");
    connect(btn,&QPushButton::clicked,this,&MileWidget::calculate);
    vlayout->addWidget(btn);
    mileEdit=new QLineEdit;
    mileEdit->setFocusPolicy(Qt::NoFocus);
    flayout->addRow("里程",mileEdit);
    vlayout->addLayout(flayout);
    pathText=new QTextBrowser;
    QScroller::grabGesture(pathText,QScroller::TouchGesture);
    vlayout->addWidget(new QLabel("经由: "));
    vlayout->addWidget(pathText);
    setLayout(vlayout);
}

void MileWidget::calculate()
{
    const QString& start=startEdit->text();
    const QString& end=endEdit->text();
    QStringList path;
    int mile=crNet.mileBetween(start,end,path);
    if(mile==-1||mile==CRNet::INF){
        QMessageBox::warning(this,"错误","始发、终到站不存在，或者不存在路径！");
        return;
    }
    emit mileCalculated(QString::number(mile));
    mileEdit->setText(QString::number(mile));
    QString p;
    for(QString& s:path) {
        p.append(s+"\n");
    }
    pathText->setPlainText(p);
}
