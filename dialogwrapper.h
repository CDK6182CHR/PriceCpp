#ifndef DIALOGWRAPPER_H
#define DIALOGWRAPPER_H
#include <QtWidgets>

class DialogWrapper : public QDialog
{
    Q_OBJECT
public:
    DialogWrapper(QWidget* widget,QWidget* parent=nullptr);
};

#endif // DIALOGWRAPPER_H
