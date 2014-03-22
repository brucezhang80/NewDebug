/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/12 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#ifndef MUSICLISTWIDGET_H
#define MUSICLISTWIDGET_H

#include <QListWidget>
#include <QListWidgetItem>
#include "MusicItemWidget.h"

class MusicListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MusicListWidget(QWidget *parent = 0);

signals:

public slots:
    void itemHoverEnter(QListWidgetItem* item);
    void itemHoverLeave(QListWidgetItem* item);

private:
    int                 m_itemHeight;
    MusicItemWidget*    m_musicItem;
};

#endif // MUSICLISTWIDGET_H
