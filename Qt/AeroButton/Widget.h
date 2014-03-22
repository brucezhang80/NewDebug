/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/20 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "AeroButton.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    AeroButton*     m_aeroBtn;

private slots:
    void connectMySql();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
