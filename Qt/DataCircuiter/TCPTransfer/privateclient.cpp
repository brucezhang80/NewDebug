#include "privateclient.h"
#include "ui_privateclient.h"

#include <QtGui>

PrivateClient::PrivateClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrivateClient)
{
    ui->setupUi(this);
    tableFormat.setBorder(0);
    ui->splitter->setStretchFactor(0, 80);
    ui->splitter->setStretchFactor(1, 20);

    connect(ui->pri_btnSendmsg, SIGNAL(clicked()), this, SLOT(sendPress()));
//    connect(ui->pri_btnSendmsg, SIGNAL(clicked()), this, SLOT());

}

PrivateClient::~PrivateClient()
{
    delete ui;
}

void PrivateClient::setRecieverNick(const QString &recverNick)
{
    ui->pri_lbNick->setText(recverNick);
}

void PrivateClient::setRecieverAddress(const QHostAddress &recverAddress)
{
    ui->pri_lbAddress->setText(recverAddress.toString());
}

void PrivateClient::setRecieverPort(const quint16 &recverPort)
{
    ui->pri_lbPort->setText(QString("%1").arg(recverPort));
}

void PrivateClient::setRecieverMsg(const QString &from, const QMap<QString,QString> &msgMap)
{
    QMapIterator<QString, QString> i(msgMap);
    while( i.hasNext() )
    {
        i.next();
        appendMessage(from,  i.value(), i.key());
    }
}

void PrivateClient::appendMessage(const QString &from, const QString &message, const QString &time)
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
//    table->mergeCells(0, 0, 1, 2);

    table->cellAt(0, 0).firstCursorPosition().insertHtml( nick );
    table->cellAt(1, 0).firstCursorPosition().insertHtml(message);

    QScrollBar *bar = ui->pri_editSent->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void PrivateClient::sendPress()
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

