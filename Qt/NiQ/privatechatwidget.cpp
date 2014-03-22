#include "privatechatwidget.h"
#include "ui_privatechatwidget.h"
#include <QTextTable>
#include <QTextCursor>
#include <QScrollBar>
#include <QTime>
#include <QMessageBox>
#include <QMenu>

#include <QDebug>

PrivateChatWidget::PrivateChatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrivateChatWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_QuitOnClose, true);

    m_tableFormat.setBorder(0);
    ui->splitter->setStretchFactor(0, 70);
    ui->splitter->setStretchFactor(1, 30);


    connect(ui->btn_priSendMsg, SIGNAL(clicked()),
            this, SLOT(sendMessage()) );
//    connect( ui->btn_priClose, SIGNAL(clicked()),
//            this, SLOT(close()) );
}

PrivateChatWidget::PrivateChatWidget(Connection *conn, QWidget *parent):
    QWidget(parent),
    ui(new Ui::PrivateChatWidget)
{
    ui->setupUi(this);
//    setAttribute(Qt::WA_DeleteOnClose);
//    setAttribute(Qt::WA_QuitOnClose, true);

    this->m_connection = conn;

    m_tableFormat.setBorder(0);
    ui->splitter->setStretchFactor(0, 70);
    ui->splitter->setStretchFactor(1, 30);

    ui->tedt_priInputText->installEventFilter(this);//为聊天窗口输入文本框中添加发送热键

    QMenu *menu=new QMenu(this);
    menu->addMenu(tr("按Enter键发送消息"));
    menu->addMenu(tr("按Ctrl+Enter键发送消息"));

    ui->btn_hotKey->setPopupMode(QToolButton::MenuButtonPopup);

    connect(ui->btn_priSendMsg, SIGNAL(clicked()), this, SLOT(sendMessage()) );
    emit opened();
}

PrivateChatWidget::~PrivateChatWidget()
{
    emit closed();
    delete ui;
}

void PrivateChatWidget::closeEvent(QCloseEvent *e)
{
    this->close();
    this->deleteLater();
    e->ignore();
}

bool PrivateChatWidget::eventFilter(QObject *obj, QEvent *ev)
{
    Q_ASSERT(obj == ui->tedt_priInputText);
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

void PrivateChatWidget::setRevieverName(const QString &name)
{

    m_revieverName = name;
}

void PrivateChatWidget::setRevieverAddress(const QHostAddress &address)
{

    m_revieverAddress = address;
}

void PrivateChatWidget::setRevieverPort(const quint16 &port)
{
    m_revieverPort = port;
}

// send message slot
void PrivateChatWidget::appendMessage(const QString &from, const QString &message, const QString &time)
{
    if (from.isEmpty() || message.isEmpty())
        return;

    QString nick = "<font color=blue>[" + from + "] " + time + "</font>";
    if(from == m_myName)
    {
        nick = "<font color=green>[" + from + "] " + time + "</font>";
    }
    QTextCursor cursor(ui->tedt_priOutputText->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(2, 1, m_tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertHtml( nick );
    table->cellAt(1, 0).firstCursorPosition().insertHtml(message);
    QScrollBar *bar = ui->tedt_priOutputText->verticalScrollBar();
    bar->setValue(bar->maximum());
}

// click send button slot
void PrivateChatWidget::sendMessage()
{
    if (ui->tedt_priInputText->toPlainText().isEmpty())
        return;
    QString text = ui->tedt_priInputText->toHtml();
    QTime time;
    QString curTime = time.currentTime().toString("HH:mm:ss");
    appendMessage(m_myName, text, curTime);
    ui->tedt_priInputText->clear();

    if(m_myName == m_revieverName)
        return;

    this->m_connection->sendMessage(text);

}

