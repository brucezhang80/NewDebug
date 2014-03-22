#include "CalendarWidget.h"

#include <QPalette>
#include <QColor>
#include <QTableView>
#include <QHeaderView>

#include <QDebug>

CalendarWidget::CalendarWidget(QWidget *parent) :
    QCalendarWidget(parent)
{
    // hide the page change widget
    this->setNavigationBarVisible(false);

    // hide vertical header that displays ISO week numbers
    this->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);

    // show grid
    this->setGridVisible(true);

    //Monday is first day of week
    this->setFirstDayOfWeek(Qt::Monday);


    QPalette pal = palette();
    pal.setBrush(QPalette::Base, QBrush(QColor(0xff,0xff,0xff,0)));
    setPalette(pal);

//    QTableView* view = this->findChild<QTableView*>( QString("qt_calendar_calendarview"));
//    if( view )
//    {
//        QHeaderView* hHeader = view->horizontalHeader();
//        QPalette p = hHeader->palette();
//        p.setBrush(QPalette::Base, QBrush(QColor(0xff,0xff,0xff,0)));
//        hHeader->setPalette(p);
//    }
}
