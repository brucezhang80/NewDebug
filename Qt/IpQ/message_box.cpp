#include "message_box.h"
#include "macai.h"
#include "system.h"
#include "preferences.h"
#include "systemtray.h"
#include "ipq.h"
#include "message.h"
#include "send_message.h"
#include "message_thread.h"
#include "transfer_file_window.h"

#include <QGridLayout>
#include <QHeaderView>
#include <QListWidget>
#include <QTimer>
#include <QDesktopWidget>
#include <QToolTip>
#include <QMouseEvent>
#include <QtEvents>
#include <QRegExp>
#include <QLabel>

MessageBox::MessageBox(QWidget *parent) :
    QDialog(parent)
{
    setupUi();
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);
    setWindowFlags(windowFlags() | Qt::Popup);
    this->setAttribute(Qt::WA_DeleteOnClose);

    QRect rect = Macai::systemTray->geometry();
    moveTo(rect.x(), rect.y());
    this->raise();
    this->activateWindow();

    this->installEventFilter(m_userMsgList);
    this->installEventFilter(this);

    retranslateUi();

    m_closeTimer = new QTimer(this);
    connect( m_closeTimer, SIGNAL(timeout()), this, SLOT(hide()) );
    m_closeTimer->start(4000);

    setConnect();

    m_isSendFile = false;
}

MessageBox::~MessageBox()
{
    freeUi();
}

void MessageBox::retranslateUi()
{
    m_tips = tr(" sent a file to you");
    this->setWindowTitle(QApplication::translate("MessageBox", "Message Box", 0, QApplication::UnicodeUTF8));
}

void MessageBox::setupUi()
{
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("MessageBox"));
    this->resize(220, 120);
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    m_userMsgList = new QListWidget(this);
    m_userMsgList->setObjectName(QString::fromUtf8("userMsgList"));
    m_gridLayout->addWidget(m_userMsgList, 0, 0, 1, 1);
}

void MessageBox::freeUi()
{
    m_userMsgList->deleteLater();
}

void MessageBox::setConnect()
{
    connect( m_userMsgList, SIGNAL(itemClicked(QListWidgetItem*)),
             this, SLOT(showMessageWindow(QListWidgetItem*)) );
}

bool MessageBox::eventFilter(QObject *obj, QEvent *event)
{
//    QListWidget *list = static_cast<QListWidget*>(obj);
//    MessageBox *_this = static_cast<MessageBox*>(obj);

//    if( list !=  m_userMsgList || _this != this) return false;
    switch( event->type() )
    {
    case QEvent::Enter:
    {
        m_closeTimer->stop();
    };break;

    case QEvent::Leave:
    {
        m_closeTimer->start(3000);
    };break;
    }

    return QObject::eventFilter(obj, event);
}

void MessageBox::closeEvent(QCloseEvent *e)
{
    this->hide();
    e->ignore();
}

/*���ϵͳ������λ��*/
MessageBox::TaskbarPos MessageBox::taskbarDirect()
{
    MessageBox::TaskbarPos taskbarPos;
    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();//ȡ����Ļ��С
    QRect workRect = desktopWidget->availableGeometry();//ȡ�ÿ�����Ļ��С��ȥ����������С��


    // ��Ļ�߶� - �������߶� = �������߶�, �����ֵ����0.������������������
    // ������ֱ�ӱȽ����ߵĸ߶ȣ���ȼ���
    if ( screenRect.height() == workRect.height() )
    {
        // ������������Ͻ�x���������Ļ���Ͻ�x���꣬ ���������ұ�
        if ( screenRect.topLeft().x() == workRect.topLeft().x() )
        {
            //qDebug() << "������λ����Ļ�ұ�: " << screenRect.topRight().x() << workRect.topRight().x();
            taskbarPos = MessageBox::Right;
        }
        else
        {
            //qDebug() << "������λ����Ļ���: " << workRect.topLeft().x() << workRect.topLeft().x();
            taskbarPos = MessageBox::Left;
        }
    }
    // �����ȡ�˵��������λ������Ļ�Ϸ����·�
    else /*if(screenRect.width() == workRect.width())*/
    {
        // ������������½�y���������Ļ���½�y���꣬ ���������Ϸ�
        if ( screenRect.bottomRight().y() == workRect.bottomRight().y() )
        {
            //qDebug() << "������λ����Ļ�Ϸ�: " << screenRect.topRight().y() << workRect.topRight().y();
            taskbarPos = MessageBox::Top;
        }
        else
        {
            //qDebug() << "������λ����Ļ�·�: " << screenRect.bottomRight().y() << workRect.bottomRight().y();
            taskbarPos = MessageBox::Bottom;
        }
    }

    return taskbarPos;
}

// ��Ϣ��������������ʾ��λ��
void MessageBox::moveTo(const int &xbase, const int &ybase, MessageBox::TaskbarPos taskbarPos)
{
    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();//ȡ����Ļ��С
    QRect workRect = desktopWidget->availableGeometry();//ȡ�ÿ�����Ļ��С��ȥ����������С��
    QRect thisRect = this->geometry();  //ȡ�ô��ڱ���Ĵ�С,�����frameGeomety�����������ڱ���߶�
    QPoint thisScreenPos = this->mapToGlobal( QPoint(thisRect.bottomRight().x(), thisRect.bottomRight().y()) ); // ��������ת����Ļ����

    qDebug() << thisRect.bottomRight().x() << thisRect.bottomRight().y() << thisScreenPos.x() << thisScreenPos.y();
    // qt�ĵ���˵��bottomRight����Щ������ Note that for historical reasons this function returns QPoint(left() + width() -1, top() + height() - 1).
    // ������ȥ1����λ�����ˣ���������ıȽ���Ҫ�ټ�����һ����λ������
    switch(taskbarPos)
    {
    case MessageBox::Left:
    {
//        int thisWidth = thisRect.width();
//        int taskbarWidth = screenRect.width() - workRect.width(); //���������
        // �����ǰ���ڳ�����Ļ���⣨���³�������y������Ҫ����,����y����Ϊ��Ļy���꣬x�ټ�����������ȡ�
//        qDebug() <<thisScreenPos.y() << screenRect.bottomRight().y();
        if(thisScreenPos.y() > screenRect.bottomRight().y()+1)
        {
            int thisY = screenRect.bottomLeft().y() - thisRect.height();
            this->move(workRect.bottomLeft().x(), thisY);
        }
        else
            this->move(workRect.bottomLeft().x(), ybase);
    };break;

    case MessageBox::Right:
    {
        int thisWidth = thisRect.width();
//        int taskbarWidth = screenRect.width() - workRect.width(); //���������
        // �����ǰ���ڳ�����Ļ���⣨���³�������y������Ҫ����,����y����Ϊ��Ļy���꣬x�ټ�ȥ��������Ⱥ������ȡ�

//        qDebug() << thisScreenPos.y() << screenRect.bottomRight().y();

        if(thisScreenPos.y() > screenRect.bottomRight().y()+1)
        {
            int thisY = screenRect.bottomRight().y() - thisRect.height();
            this->move(workRect.bottomRight().x() - thisWidth-8, thisY);
        }
        else
            this->move(workRect.bottomRight().x() - thisWidth-8, ybase);
    };break;

    case MessageBox::Top:
    {
//        int thisHeight = thisRect.height();
//        int taskbarHeight = screenRect.height() - workRect.height(); //�������߶�
        // �����ǰ���ڳ�����Ļ���⣨���ҳ�������x������Ҫ����,����x����Ϊ��Ļx����, y�ټ����������߶ȡ�
        if(thisScreenPos.x() > screenRect.bottomRight().x()+1)
        {
            int thisX = screenRect.topRight().x() - thisRect.width();
            this->move(thisX, workRect.topRight().y());
        }
        else
            this->move(xbase, workRect.topRight().y());
    };break;

    case MessageBox::Bottom:
    {
        int thisHeight = thisRect.height();
        int taskbarHeight = screenRect.height() - workRect.height(); //�������߶�

        // �����ǰ���ڳ�����Ļ���⣨���ҳ�������x������Ҫ����,����x����Ϊ��Ļx����, y�ټ�ȥ��������߶Ⱥ��������߶ȡ�
//        qDebug() << thisScreenPos.x() << screenRect.bottomRight().x();

        if(thisScreenPos.x() > screenRect.bottomRight().x()+1)
        {
            int thisX = screenRect.bottomRight().x() - thisRect.width();
            this->move(thisX, workRect.bottomRight().y() - thisHeight - taskbarHeight);
        }
        else
            this->move(xbase, workRect.bottomRight().y() - thisHeight - taskbarHeight);
    };break;

    default:
        break;
    }
}

// ��Ϣ��������������ʾ��λ��
void MessageBox::moveTo(const int &xbase, const int &ybase)
{
    MessageBox::TaskbarPos taskbarPos = taskbarDirect();
    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();//ȡ����Ļ��С
    QRect workRect = desktopWidget->availableGeometry();//ȡ�ÿ�����Ļ��С��ȥ����������С��
    QRect thisRect = this->geometry();  //ȡ�ô��ڱ���Ĵ�С,�����frameGeomety�����������ڱ���߶�
//    QPoint thisScreenPos = this->mapToGlobal( QPoint(thisRect.bottomRight().x(), thisRect.bottomRight().y()) ); // ��������ת����Ļ����
    QPoint thisScreenPos = this->mapToGlobal( thisRect.bottomRight() );
//    qDebug() << thisRect.bottomRight().x() << thisRect.bottomRight().y() << thisScreenPos.x() << thisScreenPos.y();

    // qt�ĵ���˵��bottomRight����Щ������ Note that for historical reasons this function returns QPoint(left() + width() -1, top() + height() - 1).
    // ������ȥ1����λ�����ˣ���������ıȽ���Ҫ�ټ�����һ����λ������
    switch(taskbarPos)
    {
    case MessageBox::Left:
    {
//        int thisWidth = thisRect.width();
//        int taskbarWidth = screenRect.width() - workRect.width(); //���������
        // �����ǰ���ڳ�����Ļ���⣨���³�������y������Ҫ����,����y����Ϊ��Ļy���꣬x�ټ�����������ȡ�
//        qDebug() <<thisScreenPos.y() << screenRect.bottomRight().y();
        if(thisScreenPos.y() > screenRect.bottomRight().y()+1)
        {
            int thisY = screenRect.bottomLeft().y() - thisRect.height();
            this->move(workRect.bottomLeft().x(), thisY-thisRect.height()/2);
        }
        else
            this->move(workRect.bottomLeft().x(), ybase-thisRect.height()/2);
    };break;

    case MessageBox::Right:
    {
        int thisWidth = thisRect.width();
//        int taskbarWidth = screenRect.width() - workRect.width(); //���������
        // �����ǰ���ڳ�����Ļ���⣨���³�������y������Ҫ����,����y����Ϊ��Ļy���꣬x�ټ�ȥ��������Ⱥ������ȡ�

//        qDebug() << thisScreenPos.y() << screenRect.bottomRight().y();

        if(thisScreenPos.y() > screenRect.bottomRight().y()+1)
        {
            int thisY = screenRect.bottomRight().y() - thisRect.height();
            this->move(workRect.bottomRight().x() - thisWidth-8, thisY-thisRect.height()/2);
        }
        else
            this->move(workRect.bottomRight().x() - thisWidth-8, ybase-thisRect.height()/2);
    };break;

    case MessageBox::Top:
    {
//        int thisHeight = thisRect.height();
//        int taskbarHeight = screenRect.height() - workRect.height(); //�������߶�
        // �����ǰ���ڳ�����Ļ���⣨���ҳ�������x������Ҫ����,����x����Ϊ��Ļx����, y�ټ����������߶ȡ�
        if(thisScreenPos.x() > screenRect.bottomRight().x()+1)
        {
            int thisX = screenRect.topRight().x() - thisRect.width();
            this->move(thisX-thisRect.width()/2, workRect.topRight().y());
        }
        else
            this->move(xbase-thisRect.width()/2, workRect.topRight().y());
    };break;

    case MessageBox::Bottom:
    {
        int thisHeight = thisRect.height();
        int taskbarHeight = screenRect.height() - workRect.height(); //�������߶�

        // �����ǰ���ڳ�����Ļ���⣨���ҳ�������x������Ҫ����,����x����Ϊ��Ļx����, y�ټ�ȥ��������߶Ⱥ��������߶ȡ�
//        qDebug() <<"------" <<xbase<< screenRect.bottomRight().x() - thisRect.width() << workRect.bottomRight().y() - thisHeight - taskbarHeight;

//        qDebug() <<"------" << thisScreenPos.x() << screenRect.bottomRight().x();
        if(thisScreenPos.x() > screenRect.bottomRight().x()+1)
        {
            int thisX = screenRect.bottomRight().x() - thisRect.width();
            this->move(thisX-thisRect.width()/2, workRect.bottomRight().y() - thisHeight - taskbarHeight);
        }
        else
            this->move(xbase-thisRect.width()/2, workRect.bottomRight().y() - thisHeight - taskbarHeight);
    };break;

    default:
        break;
    }
}

void MessageBox::insertUserMsgItem(const QString &name, const QString &ip)
{
    m_isSendFile = false;
    QString text = name+"("+ip+")";
    QList<QListWidgetItem *> items = m_userMsgList->findItems(text, Qt::MatchCaseSensitive);
    if( items.count() > 0 )
    {
        return;
    }
    new QListWidgetItem(text, m_userMsgList);
}

void MessageBox::insertUserMsgItem(const QString &/*name*/, const int &/*msgCount*/)
{
}

void MessageBox::sendFileMessage(const QString &from, const QString &ip)
{
    m_isSendFile = true;
    QString text;
    text =  from + "("+ ip +")" + m_tips;
    new QListWidgetItem(text, m_userMsgList);
}

bool MessageBox::hasMessage() const
{
    if( m_userMsgList->count() == 0)
        return false;

    return true;
}

void MessageBox::show()
{
    if( hasMessage() )
        QDialog::show();
}

// ������Ϣ���ӵ���Ϣ�б���,��ʾ��Ϣ����
void MessageBox::showMessageWindow(QListWidgetItem *item)
{
    QString user = item->text();
    if ( user.contains(m_tips) )
    {
        Macai::transferFileWindow->show();
        Macai::transferFileWindow->setCurrentPage(1);
    }
    else
    {
        // ȡip��ַ
        QRegExp rx("((?:(?:25[0-5]|2[0-4]\\d|[01]?\\d?\\d)\\.){3}(?:25[0-5]|2[0-4]\\d|[01]?\\d?\\d))");
        rx.indexIn(user);
        QStringList list = rx.capturedTexts();
        emit readMessage(list.at(1));
    }

    delete item;
    if( m_userMsgList->count() == 0)
    {
        m_closeTimer->stop();
        this->hide();   // ��Ҫ�ر���Ϣ����
    }
//    quint32 flags = 0;
//    flags |= IPQ_READ_MESSAGE;
//    SendMessage sendMsg = SendMessage( QHostAddress(list.at(1)), IPQ_DEFAULTPORT, "Read", "", flags);
//    Macai::messageThread->addSendMsg( Message(sendMsg) );
}
