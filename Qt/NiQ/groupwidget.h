#ifndef GROUPWIDGET_H
#define GROUPWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QTreeWidgetItem>

namespace Ui {
class GroupWidget;
}

class GroupWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit GroupWidget(QWidget *parent = 0);
    ~GroupWidget();
    
    void setGroupName(const QString& groupname);
    QString groupName() const;

    void setGroupItem(QTreeWidgetItem* item);
    QTreeWidgetItem* groupItem() const;

signals:
    void showSendFileWidget();

private slots:
    void sendFiles();

protected:
    void mouseDoubleClickEvent(QMouseEvent *);

private:
    Ui::GroupWidget *ui;
    QString m_groupName;
    QTreeWidgetItem* m_groupItem;
};

#endif // GROUPWIDGET_H
