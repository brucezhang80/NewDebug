#ifndef PERSONWIDGET_H
#define PERSONWIDGET_H

#include <QWidget>

namespace Ui {
class PersonWidget;
}

class PersonWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PersonWidget(QWidget *parent = 0);
    ~PersonWidget();
    void    setUserName(const QString& username);
    QString userName() const{return m_userName;}
    void    setlocalHostName(const QString& localhostname);
    QString localHostName() const;
    void    setUserIp(const QString& userip);
    QString userIp() const {return m_userIp;}
    void    setUserPort(const QString& userport);
    QString userPort() const{return m_userPort;}
    void    setUserAvatar(const QString& pixmapfilename);
    QString userAvatar() const {return m_userAvatar;}
    void    setUserGroup(const QString& groupname){m_userGroup = groupname;}
    QString userGroup() const {return m_userGroup;}
    
private:
    Ui::PersonWidget    *ui;
    QString             m_localHostName;
    QString             m_userName;
    QString             m_userIp;
    QString             m_userPort;
    QString             m_userAvatar;
    QString             m_userGroup;

};

#endif // PERSONWIDGET_H
