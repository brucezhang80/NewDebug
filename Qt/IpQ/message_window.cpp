#include <QtGui>

#include "system.h"
#include "message_window.h"
#include "send_message.h"
#include "ipq.h"
#include "constants.h"
#include "macai.h"
#include "message_thread.h"
#include "configuration_page.h"

MessageWindow::MessageWindow(QWidget *parent) :
    QWidget(parent)
{
    setupUi();
    setAttribute(Qt::WA_DeleteOnClose, true);
//    setWindowFlags( Qt::WindowStaysOnTopHint );
//    setWindowFlags(windowFlags() | Qt::Window |  Qt::WindowMaximizeButtonHint|Qt::WindowMinimizeButtonHint);

    tedt_msgInputText->installEventFilter(this);
    m_tableFormat.setBorder(0);

    createSendPopmenu();
    setConnect();
}

MessageWindow::MessageWindow(Message msg, QWidget *parent)
    :QWidget(parent), m_message(msg)
{
    setupUi();
    setTheme();

    m_hostName = msg->owner().name();
    m_ip = msg->owner().ip();
    QString winTitle = "Chating with [" + m_hostName + "] ip:" + m_ip;
    setWindowTitle(winTitle);
    setAttribute(Qt::WA_DeleteOnClose, true);
//    setWindowFlags( Qt::WindowStaysOnTopHint );
//    setWindowFlags(windowFlags() | Qt::Window | Qt::WindowMaximizeButtonHint|Qt::WindowMinimizeButtonHint);

    m_tableFormat.setBorder(0);
    splitter->setStretchFactor(0, 70);
    splitter->setStretchFactor(1, 30);

    setUserInfo(msg->owner().name(), msg->owner().loginName(), msg->owner().ip(), msg->owner().group());

    tedt_msgInputText->installEventFilter(this);

    createSendPopmenu();
    setConnect();
}

MessageWindow::MessageWindow(QString hostName, QString ip, QString group, QWidget *parent)
    :QWidget(parent),m_hostName(hostName), m_ip(ip), m_group(group)
{
    setupUi();
    setTheme();

    QString winTitle = "Chating with [" + hostName + "] ip:" + ip;
    setWindowTitle(winTitle);
    setAttribute(Qt::WA_DeleteOnClose, true);
//    setWindowFlags( Qt::WindowStaysOnTopHint );

    m_tableFormat.setBorder(0);
    splitter->setStretchFactor(0, 70);
    splitter->setStretchFactor(1, 30);
    setUserInfo(m_hostName, m_hostName, m_ip, m_group);

    tedt_msgInputText->installEventFilter(this);

    createSendPopmenu();
    setConnect();
}

MessageWindow::~MessageWindow()
{
    freeUi();
}

void MessageWindow::setTheme(const QString &themeFile)
{
    QFile qss(themeFile);
    if( !qss.open(QFile::ReadOnly) )
    {
        return;
    }
    this->setStyleSheet(qss.readAll());
    qss.close();
}

void MessageWindow::setTheme()
{
    QString theme = Macai::configurationPage->currentTheme();
    QFile qss(theme);
    if( !qss.open(QFile::ReadOnly) )
    {
        return;
    }
    this->setStyleSheet(qss.readAll());
    qss.close();
}

void MessageWindow::sendMessage()
{
    if (tedt_msgInputText->toPlainText().isEmpty())
        return;
    QString text = tedt_msgInputText->toHtml();
    QTime time;
    QString curTime = time.currentTime().toString("HH:mm:ss");

    QString myName = "<font color=green>[" + System::hostName() + "] " + curTime + "</font>";

    appendMessage(myName, text);
    tedt_msgInputText->clear();

    quint32 flags = 0;
    flags |= IPQ_SEND_MESSAGE;
    SendMessage sendMsg = SendMessage( QHostAddress(m_ip), IPQ_DEFAULTPORT, text, "", flags);
    Macai::messageThread->addSendMsg( Message(sendMsg) );
}

void MessageWindow::recvMessage(Message msg)
{
    QString from = msg->owner().name(); // host name, login name
//    QString ip = msg->ip();
    QString text = msg->additionalInfo();
    QString extend = msg->extendedInfo();
//    qDebug() << "extend:" << extend;
    QTime time;
    QString sendTime = time.currentTime().toString("HH:mm:ss");
    QString name = "<font color=blue>[" + from + "] " + sendTime + "</font>";

    appendMessage(name, text);
}

void MessageWindow::closeEvent(QCloseEvent *ce)
{
    //不隐藏,直接关掉
    //this->hide();

    emit removeMessageWindow();
    ce->accept();
}

bool MessageWindow::eventFilter(QObject *obj, QEvent *ev)
{
    Q_ASSERT(obj == tedt_msgInputText);
    if (ev->type() == QEvent::KeyPress)
    {
        QKeyEvent *e = static_cast<QKeyEvent*>(ev);

        if(e->key() == Qt::Key_Return && (e->modifiers() & Qt::ControlModifier))
        {
            sendMessage();
            return true;
        }
    }

    return QObject::eventFilter(obj,ev);
}

void MessageWindow::freeUi()
{
    tedt_msgOutputText->deleteLater();
    btn_msgShaking->deleteLater();
    btn_msgSetFont->deleteLater();
    btn_msgSendFile->deleteLater();
    btn_msgSendImg->deleteLater();
    tedt_msgInputText->deleteLater();
    btn_msgClose->deleteLater();
    btn_msgSendMsg->deleteLater();
//    btn_hotKey->deleteLater();
}

void MessageWindow::setupUi()
{
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("MessageWindow"));
    this->setWindowModality(Qt::NonModal);
    this->resize(520, 420);

    gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    splitter_3 = new QSplitter(this);
    splitter_3->setOrientation(Qt::Horizontal);
    splitter_3->setOpaqueResize(false);
    splitter_3->setHandleWidth(1);
    splitter_3->setChildrenCollapsible(false);

    splitter = new QSplitter(splitter_3);
    splitter->setOrientation(Qt::Vertical);

    tedt_msgOutputText = new QTextEdit(splitter);
    tedt_msgOutputText->setObjectName(QString::fromUtf8("tedt_msgOutputText"));
    tedt_msgOutputText->setReadOnly(true);
    splitter->addWidget(tedt_msgOutputText);

    widget = new QWidget(splitter);
    verticalLayout = new QVBoxLayout(widget);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout_2 = new QGridLayout();
    btn_msgShaking = new QToolButton(widget);
    btn_msgShaking->setObjectName(QString::fromUtf8("btn_msgShaking"));

    gridLayout_2->addWidget(btn_msgShaking, 0, 3, 1, 1);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout_2->addItem(horizontalSpacer, 0, 4, 1, 1);

    btn_msgSetFont = new QToolButton(widget);
    btn_msgSetFont->setObjectName(QString::fromUtf8("btn_msgSetFont"));

    gridLayout_2->addWidget(btn_msgSetFont, 0, 0, 1, 1);

    btn_msgSendFile = new QToolButton(widget);
    btn_msgSendFile->setObjectName(QString::fromUtf8("btn_msgSendFile"));

    gridLayout_2->addWidget(btn_msgSendFile, 0, 2, 1, 1);

    btn_msgSendImg = new QToolButton(widget);
    btn_msgSendImg->setObjectName(QString::fromUtf8("btn_msgSendImg"));

    gridLayout_2->addWidget(btn_msgSendImg, 0, 1, 1, 1);


    verticalLayout->addLayout(gridLayout_2);

    tedt_msgInputText = new QTextEdit(widget);
    tedt_msgInputText->setObjectName(QString::fromUtf8("tedt_msgInputText"));

    verticalLayout->addWidget(tedt_msgInputText);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setContentsMargins(-1, -1, 9, -1);
    horizontalSpacer_2 = new QSpacerItem(234, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer_2);

    btn_msgClose = new QPushButton(widget);
    btn_msgClose->setObjectName(QString::fromUtf8("btn_msgClose"));
    btn_msgClose->setMaximumSize(QSize(50, 16777215));

    horizontalLayout->addWidget(btn_msgClose);

//    splitter_2 = new QSplitter(widget);
//    splitter_2->setOrientation(Qt::Horizontal);
//    splitter_2->setOpaqueResize(false);
//    splitter_2->setHandleWidth(1);
//    splitter_2->setChildrenCollapsible(false);
    btn_msgSendMsg = new QToolButton(widget);
    btn_msgSendMsg->setObjectName(QString::fromUtf8("btn_msgSendMsg"));
    btn_msgSendMsg->setMinimumSize(QSize(90, 0));
    btn_msgSendMsg->setMaximumSize(QSize(90, 16777215));
    btn_msgSendMsg->setPopupMode(QToolButton::MenuButtonPopup);
//    splitter_2->addWidget(btn_msgSendMsg);
//    btn_hotKey = new QToolButton(splitter_2);
//    btn_hotKey->setObjectName(QString::fromUtf8("btn_hotKey"));
//    btn_hotKey->setMinimumSize(QSize(10, 0));
//    btn_hotKey->setMaximumSize(QSize(10, 16777215));
//    splitter_2->addWidget(btn_hotKey);

    horizontalLayout->addWidget(btn_msgSendMsg);


    verticalLayout->addLayout(horizontalLayout);

    splitter->addWidget(widget);
    splitter_3->addWidget(splitter);
    widget1 = new QWidget(splitter_3);
    verticalLayout_2 = new QVBoxLayout(widget1);
    verticalLayout_2->setContentsMargins(0, 0, 0, 0);
    userInfoLabel = new QLabel(widget1);
    userInfoLabel->setObjectName(QString::fromUtf8("userInfoLabel"));
    userInfoLabel->setMinimumSize(QSize(100, 100));

    verticalLayout_2->addWidget(userInfoLabel);

    fileTransferTree = new QTreeWidget(widget1);
    fileTransferTree->setObjectName(QString::fromUtf8("fileTransferTree"));
    fileTransferTree->setMaximumSize(QSize(16777215, 16777215));

    verticalLayout_2->addWidget(fileTransferTree);

    splitter_3->addWidget(widget1);

    gridLayout->addWidget(splitter_3, 0, 0, 1, 1);

    splitter->setStretchFactor(0, 70);
    splitter->setStretchFactor(1, 30);
    splitter_3->setStretchFactor(0, 60);
    splitter_3->setStretchFactor(1, 40);

    retranslateUi();
}

void MessageWindow::retranslateUi()
{
    btn_msgShaking->setText(QApplication::translate("MessageWindow", "Shaking", 0, QApplication::UnicodeUTF8));
    btn_msgSetFont->setText(QApplication::translate("MessageWindow", "Font", 0, QApplication::UnicodeUTF8));
    btn_msgSendFile->setText(QApplication::translate("MessageWindow", "File", 0, QApplication::UnicodeUTF8));
    btn_msgSendImg->setText(QApplication::translate("MessageWindow", "Image", 0, QApplication::UnicodeUTF8));
    btn_msgClose->setText(QApplication::translate("MessageWindow", "Close", 0, QApplication::UnicodeUTF8));
    btn_msgSendMsg->setText(QApplication::translate("MessageWindow", "Send(Enter)", 0, QApplication::UnicodeUTF8));
//    btn_hotKey->setText(QApplication::translate("MessageWindow", "\342\206\223", 0, QApplication::UnicodeUTF8));
    userInfoLabel->setText(QApplication::translate("MessageWindow", "User Information", 0, QApplication::UnicodeUTF8));
}

void MessageWindow::setConnect()
{
    connect(btn_msgSendMsg, SIGNAL(clicked()), this, SLOT(sendMessage()) );
    connect(btn_msgClose, SIGNAL(clicked()), this, SLOT(close()) );

}

void MessageWindow::appendMessage(const QString &from, const QString &msg)
{
    if (from.isEmpty() || msg.isEmpty())
        return;

    QTextCursor cursor(tedt_msgOutputText->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(2, 1, m_tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertHtml( from );
    table->cellAt(1, 0).firstCursorPosition().insertHtml(msg);
    QScrollBar *bar = tedt_msgOutputText->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void MessageWindow::setUserInfo(const QString &name, const QString &hostnaem, const QString &ip, const QString &group)
{
    QString strLb = tr("User Name:") + name\
            + tr("\nLogin Name:") + hostnaem\
            + tr("\nIP:") + ip\
            + tr("\nGroup:") + group + "\n";
    userInfoLabel->setText(strLb);
}

void MessageWindow::createSendPopmenu()
{
    QMenu *menu = new QMenu(this);
    m_enterAct = new QAction(this);
    menu->addAction(m_enterAct);
    m_enterAct->setText(tr("&Enter send message"));
    connect(m_enterAct, SIGNAL(triggered()), this, SLOT(setEnterHotkey()));

    m_ctrlEnterAct = new QAction(this);
    menu->addAction(m_ctrlEnterAct);
    m_ctrlEnterAct->setText(tr("&Ctrl + Enter send message"));
    connect(m_ctrlEnterAct, SIGNAL(triggered()), this, SLOT(setCtrlEnterHotkey()));

    QRect rect = btn_msgSendMsg->geometry();
    menu->popup(rect.bottomRight());
    btn_msgSendMsg->setMenu(menu);
}

void MessageWindow::setEnterHotkey()
{

}

void MessageWindow::setCtrlEnterHotkey()
{

}
