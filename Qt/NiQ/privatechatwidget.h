#ifndef PRIVATECHATWIDGET_H
#define PRIVATECHATWIDGET_H

#include <QtNetwork>
#include <QWidget>
#include <QTextTableFormat>
#include "common/connection.h"
#include "userwidget.h"
#include <QCloseEvent>

namespace Ui {
class PrivateChatWidget;
}

class PrivateChatWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PrivateChatWidget(QWidget *parent = 0);
    explicit PrivateChatWidget(Connection *conn, QWidget *parent = 0);

    ~PrivateChatWidget();
    void setSenderName(const QString& nick){ m_myName = nick;}

    Connection* getConnection()const
    {
        return m_connection;
    }
    void setRevieverName(const QString &name);
    void setRevieverAddress(const QHostAddress &address);
    void setRevieverPort(const quint16 &port);

signals:
    void closed();      //�����촰�ڹرշ�����Ϣ:֪ͨ��Ҫ������Ϣ������ͷ��
    void opened();      //�����촰�ڴ򿪷�����Ϣ:��ʾ������Ϣ�ͽ���ͷ������

public slots:
    void appendMessage(const QString &from, const QString &time, const QString& message);
    void sendMessage();

protected:
    void closeEvent(QCloseEvent *e);
    bool eventFilter(QObject *, QEvent *);

private:
    Connection              *m_connection;
    Ui::PrivateChatWidget   *ui;
    QTextTableFormat        m_tableFormat;
    QString                 m_myName;

    QString                 m_revieverName;
    QHostAddress            m_revieverAddress;
    quint16                 m_revieverPort;

    quint16                 m_blockSize;
};

#endif // PRIVATECHATWIDGET_H
