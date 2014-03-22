#include "sendfiledlg.h"
#include "ui_sendfiledlg.h"
#include "fileclient.h"
#include <QStringList>

SendFileDlg::SendFileDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendFileDlg)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->splitter->setStretchFactor(0, 70);
    ui->splitter->setStretchFactor(1, 30);
    ui->usersWidget->setIconSize(QSize(24,24));

    connect( ui->btn_groupSendFile, SIGNAL(clicked()), this, SLOT(startSendFile()) );
}

SendFileDlg::~SendFileDlg()
{
    delete ui;
}

void SendFileDlg::setGroupUsers(const QList<UserWidget *> &userlist)
{
    m_userList.clear();
    foreach(UserWidget* user, userlist)
    {
        QString name = user->userName();
        if(!name.isEmpty())
        {
            QTreeWidgetItem* item = new QTreeWidgetItem(ui->usersWidget);
            item->setIcon(0, QIcon(user->userAvatar()));
            item->setText(0, name);

            m_userList.append(user);
        }
    }
}

void SendFileDlg::startSendFile()
{
    qDebug() << " startSendFile";

    int _size = ui->filesWidget->fileJobs().size();
    if(_size == 0) return;
    QString msg;
    for(int i = 0; i < _size; ++i)
    {
        QString filename = ui->filesWidget->fileJobs().at(i).fileName;
        msg += filename+"\n";
    }

    // 这里只需要发送一个开始传输文件的信号,在文件传输类中使用线程进行文件传输与接收
    foreach(UserWidget* user, m_userList)
    {
        user->userConnection()->sendMessage(msg);
    }
}
