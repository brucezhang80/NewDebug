#ifndef USERITEMWIDGET_H
#define USERITEMWIDGET_H

#include <QWidget>
#include <QEvent>

class QVariant;
class QAction;
class QApplication;
class QButtonGroup;
class QGridLayout;
class QHeaderView;
class QLabel;
class QSpacerItem;
class QToolButton;
class QVBoxLayout;
class QHBoxLayout;
class QWidget;
class QHostAddress;
class QTreeWidgetItem;

class UserItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserItemWidget(QWidget *parent = 0);
    UserItemWidget(QString name, QString ip, QTreeWidgetItem *item = 0, QWidget *parent = 0);

    void    setNickName(const QString& nickname);
    QString nickName() const{ return m_nickName; }

    void    setLoginName(const QString& loginname);
    QString loginName() const { return m_loginName; }

    void    setHostName(const QString& hostname);
    QString hostName() const { return m_hostName; }

    void    setUserIp(const QString& userip);
    QString userIp() const { return m_userIp; }

    void    setUserPort(const QString &userport);
    QString userPort() const{ return m_userPort; }

    void    setUserAvatar(const QString& pixmapfilename);
    QString userAvatar() const { return m_userAvatar; }

    void    setOfficeTitle(const QString& officetitle);
    QString officeTitle() const { return m_officeTitle; }

    void    setUserGroup(const QString& usergroup);
    QString userGroup() const { return m_userGroup; }

    void setCurrentItem(QTreeWidgetItem *item);
    QTreeWidgetItem* currentItem(){ return m_currentItem; }
protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
signals:
    
public slots:
    
private:
    void setupUi();
    void retranslateUi();

    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *lb_userAvatar;
    QHBoxLayout *horizontalLayout_2;
    QLabel      *lb_userIp;
    QSpacerItem *horizontalSpacer;
    QToolButton *btn_userChat;
    QHBoxLayout *horizontalLayout_1;
    QLabel      *lb_nickName;
    QToolButton *btn_userManager;

    QTreeWidgetItem         *m_currentItem;
    QString                 m_nickName;
    QString                 m_loginName;
    QString                 m_hostName;
    QString                 m_userIp;
    QString                 m_userPort;
    QString                 m_userGroup;
    QString                 m_userAvatar;
    QString                 m_officeTitle;
};

#endif // USERITEMWIDGET_H
