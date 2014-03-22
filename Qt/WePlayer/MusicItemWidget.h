/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/12 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#ifndef MUSICITEMWIDGET_H
#define MUSICITEMWIDGET_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class MusicItemWidget;
}

class MusicItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MusicItemWidget(QWidget *parent = 0);

    MusicItemWidget(QListWidgetItem* item, QWidget *parent = 0);

    ~MusicItemWidget();
    void setItem(QListWidgetItem* item) { m_item = item;}
    QListWidgetItem* item() { return m_item;}

signals:
    void mouseLeave(QListWidgetItem* item);

protected:
    void leaveEvent( QEvent * event );

private:
    Ui::MusicItemWidget *ui;
    QListWidgetItem*        m_item;
};

#endif // MUSICITEMWIDGET_H
