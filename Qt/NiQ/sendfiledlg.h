#ifndef SENDFILEDLG_H
#define SENDFILEDLG_H

#include <QDialog>
#include "friendstree.h"
#include "userwidget.h"
#include "privateChatwidget.h"

//class FileClient;

namespace Ui {
class SendFileDlg;
}

class SendFileDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit SendFileDlg(QWidget *parent = 0);
    ~SendFileDlg();

    void setUserConnection();
    void setGroupUsers(const QList<UserWidget*> &userlist);

private slots:
    void startSendFile();

private:
    Ui::SendFileDlg *ui;
    QList<UserWidget*>  m_userList;

    friend class FileClient;
};

#endif // SENDFILEDLG_H
