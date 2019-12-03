#include "dialogwrapper.h"

DialogWrapper::DialogWrapper(QWidget *widget, QWidget *parent):
    QDialog(parent)
{
    QVBoxLayout* vlayout=new QVBoxLayout;
    setWindowTitle(widget->windowTitle());
    vlayout->addWidget(widget);
    QPushButton* btn=new QPushButton("关闭");
    connect(btn,&QPushButton::clicked,this,&DialogWrapper::close);
    vlayout->addWidget(btn);
    setLayout(vlayout);
}
