#ifndef USERTREEWIDGET_H
#define USERTREEWIDGET_H

#include <QTreeWidget>

class MessageWindow;
class UserItemWidget;

class UserTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit UserTreeWidget(QWidget *parent = 0);

private:
    void initTreeWidget();
    void setConnect();

signals:
    void showMsgWindow(UserItemWidget*);

private slots:
    void showMessageWindow(QTreeWidgetItem *item, int col);

private:
    QMap<QString, MessageWindow*> m_messageWindowMap;
//    QList<MessageWindow *>      m_messageWindowList;
};

#endif // USERTREEWIDGET_H
