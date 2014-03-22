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

/*获得系统任务栏位置*/
MessageBox::TaskbarPos MessageBox::taskbarDirect()
{
    MessageBox::TaskbarPos taskbarPos;
    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();//取得屏幕大小
    QRect workRect = desktopWidget->availableGeometry();//取得可用屏幕大小（去除任务栏大小）


    // 屏幕高度 - 工作区高度 = 任务栏高度, 如果差值等于0.则任务栏在左右两边
    // 或者是直接比较两者的高度，相等即可
    if ( screenRect.height() == workRect.height() )
    {
        // 如果工作区左上角x坐标等于屏幕左上角x坐标， 则任务栏右边
        if ( screenRect.topLeft().x() == workRect.topLeft().x() )
        {
            //qDebug() << "任务栏位于屏幕右边: " << screenRect.topRight().x() << workRect.topRight().x();
            taskbarPos = MessageBox::Right;
        }
        else
        {
            //qDebug() << "任务栏位于屏幕左边: " << workRect.topLeft().x() << workRect.topLeft().x();
            taskbarPos = MessageBox::Left;
        }
    }
    // 宽度相等。说明任务栏位置在屏幕上方或下方
    else /*if(screenRect.width() == workRect.width())*/
    {
        // 如果工作区右下角y坐标等于屏幕右下角y坐标， 则任务栏上方
        if ( screenRect.bottomRight().y() == workRect.bottomRight().y() )
        {
            //qDebug() << "任务栏位于屏幕上方: " << screenRect.topRight().y() << workRect.topRight().y();
            taskbarPos = MessageBox::Top;
        }
        else
        {
            //qDebug() << "任务栏位于屏幕下方: " << screenRect.bottomRight().y() << workRect.bottomRight().y();
            taskbarPos = MessageBox::Bottom;
        }
    }

    return taskbarPos;
}

// 消息盒子在任务栏显示的位置
void MessageBox::moveTo(const int &xbase, const int &ybase, MessageBox::TaskbarPos taskbarPos)
{
    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();//取得屏幕大小
    QRect workRect = desktopWidget->availableGeometry();//取得可用屏幕大小（去除任务栏大小）
    QRect thisRect = this->geometry();  //取得窗口本身的大小,如果是frameGeomety，则会包含窗口标题高度
    QPoint thisScreenPos = this->mapToGlobal( QPoint(thisRect.bottomRight().x(), thisRect.bottomRight().y()) ); // 窗口坐标转到屏幕坐标

    qDebug() << thisRect.bottomRight().x() << thisRect.bottomRight().y() << thisScreenPos.x() << thisScreenPos.y();
    // qt文档中说到bottomRight等这些函数： Note that for historical reasons this function returns QPoint(left() + width() -1, top() + height() - 1).
    // 都被送去1个单位像素了，所以这里的比较需要再加上这一个单位的像素
    switch(taskbarPos)
    {
    case MessageBox::Left:
    {
//        int thisWidth = thisRect.width();
//        int taskbarWidth = screenRect.width() - workRect.width(); //任务栏宽度
        // 如果当前窗口超出屏幕以外（向下超出），y坐标需要上移,设置y坐标为屏幕y坐标，x再加上任务栏宽度。
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
//        int taskbarWidth = screenRect.width() - workRect.width(); //任务栏宽度
        // 如果当前窗口超出屏幕以外（向下超出），y坐标需要上移,设置y坐标为屏幕y坐标，x再减去任务栏宽度和自身宽度。

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
//        int taskbarHeight = screenRect.height() - workRect.height(); //任务栏高度
        // 如果当前窗口超出屏幕以外（向右超出），x坐标需要左移,设置x坐标为屏幕x坐标, y再加上任务栏高度。
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
        int taskbarHeight = screenRect.height() - workRect.height(); //任务栏高度

        // 如果当前窗口超出屏幕以外（向右超出），x坐标需要左移,设置x坐标为屏幕x坐标, y再减去窗口自身高度和任务栏高度。
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

// 消息盒子在任务栏显示的位置
void MessageBox::moveTo(const int &xbase, const int &ybase)
{
    MessageBox::TaskbarPos taskbarPos = taskbarDirect();
    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();//取得屏幕大小
    QRect workRect = desktopWidget->availableGeometry();//取得可用屏幕大小（去除任务栏大小）
    QRect thisRect = this->geometry();  //取得窗口本身的大小,如果是frameGeomety，则会包含窗口标题高度
//    QPoint thisScreenPos = this->mapToGlobal( QPoint(thisRect.bottomRight().x(), thisRect.bottomRight().y()) ); // 窗口坐标转到屏幕坐标
    QPoint thisScreenPos = this->mapToGlobal( thisRect.bottomRight() );
//    qDebug() << thisRect.bottomRight().x() << thisRect.bottomRight().y() << thisScreenPos.x() << thisScreenPos.y();

    // qt文档中说到bottomRight等这些函数： Note that for historical reasons this function returns QPoint(left() + width() -1, top() + height() - 1).
    // 都被送去1个单位像素了，所以这里的比较需要再加上这一个单位的像素
    switch(taskbarPos)
    {
    case MessageBox::Left:
    {
//        int thisWidth = thisRect.width();
//        int taskbarWidth = screenRect.width() - workRect.width(); //任务栏宽度
        // 如果当前窗口超出屏幕以外（向下超出），y坐标需要上移,设置y坐标为屏幕y坐标，x再加上任务栏宽度。
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
//        int taskbarWidth = screenRect.width() - workRect.width(); //任务栏宽度
        // 如果当前窗口超出屏幕以外（向下超出），y坐标需要上移,设置y坐标为屏幕y坐标，x再减去任务栏宽度和自身宽度。

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
//        int taskbarHeight = screenRect.height() - workRect.height(); //任务栏高度
        // 如果当前窗口超出屏幕以外（向右超出），x坐标需要左移,设置x坐标为屏幕x坐标, y再加上任务栏高度。
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
        int taskbarHeight = screenRect.height() - workRect.height(); //任务栏高度

        // 如果当前窗口超出屏幕以外（向右超出），x坐标需要左移,设置x坐标为屏幕x坐标, y再减去窗口自身高度和任务栏高度。
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

// 单击消息盒子的消息列表项,显示消息窗口
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
        // 取ip地址
        QRegExp rx("((?:(?:25[0-5]|2[0-4]\\d|[01]?\\d?\\d)\\.){3}(?:25[0-5]|2[0-4]\\d|[01]?\\d?\\d))");
        rx.indexIn(user);
        QStringList list = rx.capturedTexts();
        emit readMessage(list.at(1));
    }

    delete item;
    if( m_userMsgList->count() == 0)
    {
        m_closeTimer->stop();
        this->hide();   // 不要关闭消息盒子
    }
//    quint32 flags = 0;
//    flags |= IPQ_READ_MESSAGE;
//    SendMessage sendMsg = SendMessage( QHostAddress(list.at(1)), IPQ_DEFAULTPORT, "Read", "", flags);
//    Macai::messageThread->addSendMsg( Message(sendMsg) );
}
