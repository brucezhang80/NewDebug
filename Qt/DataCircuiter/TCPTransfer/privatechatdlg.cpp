#include "privatechatdlg.h"
#include "ui_privatechatdlg.h"

#include <QtGui>

PrivateChatDlg::PrivateChatDlg(Client *client, Connection *connection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrivateChatDlg)
{
    ui->setupUi(this);
    tableFormat.setBorder(0);
    ui->splitter->setStretchFactor(0, 80);
    ui->splitter->setStretchFactor(1, 20);


    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowMinMaxButtonsHint;
    this->setWindowFlags( flags );
//    this->setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->pri_btnSendmsg, SIGNAL(clicked()), this, SLOT(sendPress()));
}

PrivateChatDlg::~PrivateChatDlg()
{
    delete ui;
}

void PrivateChatDlg::setRecieverNick(const QString &recverNick)
{
    ui->pri_lbNick->setText(recverNick);
}

void PrivateChatDlg::setRecieverAddress(const QHostAddress &recverAddress)
{
    ui->pri_lbAddress->setText(recverAddress.toString());
}

void PrivateChatDlg::setRecieverPort(const quint16 &recverPort)
{
    ui->pri_lbPort->setText(QString("%1").arg(recverPort));
}

void PrivateChatDlg::setRecieverMsg(const QString &from, const QMap<QString, QString> &msgMap)
{
    QMapIterator<QString, QString> i(msgMap);
    while( i.hasNext() )
    {
        i.next();
        appendMessage(from,  i.value(), i.key());
    }
}

void PrivateChatDlg::sendPress()
{
    QString text = ui->pri_editInput->toPlainText();
    if (text.isEmpty())
        return;

    QTime time;
    QString curTime = time.currentTime().toString("HH:mm:ss");
    appendMessage(myNickName, text, curTime);
    ui->pri_editInput->clear();

    emit sendMessage(text);
}

void PrivateChatDlg::appendMessage(const QString &from, const QString &message, const QString &time)
{
    if (from.isEmpty() || message.isEmpty())
        return;

    QString nick = "<font color=blue>[" + from + "] " + time + "</font>";
    if(from == myNickName)
    {
        nick = "<font color=green>[" + from + "] " + time + "</font>";
    }
    QTextCursor cursor(ui->pri_editSent->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(2, 1, tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertHtml( nick );
    table->cellAt(1, 0).firstCursorPosition().insertHtml(message);
    QScrollBar *bar = ui->pri_editSent->verticalScrollBar();
    bar->setValue(bar->maximum());
}
