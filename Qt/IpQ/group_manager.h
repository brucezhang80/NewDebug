#ifndef GROUP_MANAGER_H
#define GROUP_MANAGER_H

#include <QObject>

class QTreeWidget;
class QTreeWidgetItem;
class QPushButton;

class GroupManager : public QObject
{
    Q_OBJECT
public:
    explicit GroupManager(QObject *parent = 0);
    ~GroupManager();
    GroupManager(QString title, QTreeWidget *treeWidget = 0);

    void retranslateUi();

    QTreeWidgetItem* groupItem() const;
    void            setGroupItem(QTreeWidget* tree, const QString &title);
signals:
    
public slots:
    void createTransferFileWin(bool bShow);

private:
    QWidget* createWidget();
    QList<QTreeWidgetItem*>    groupChildren();

    QString             m_title;
    QTreeWidgetItem     *m_group;
    QTreeWidget         *m_treeWidget;
    QPushButton         *m_sendFileBtn;
};

#endif // GROUP_MANAGER_H
