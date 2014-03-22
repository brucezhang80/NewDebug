#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H

#include <QDialog>

class QListWidget;
class QGridLayout;
class Timer;
class QMouseEvent;
class QEvent;
class QListWidgetItem;
class QLabel;
class MessageBox : public QDialog
{
    Q_OBJECT
public:
    explicit MessageBox(QWidget *parent = 0);
    ~MessageBox();
    void retranslateUi();

    enum TaskbarPos
    {
        Left,
        Right,
        Top,
        Bottom
    };
    MessageBox::TaskbarPos taskbarDirect();
    void moveTo(const int &xbase, const int &ybase, MessageBox::TaskbarPos taskbarPos);
    void moveTo(const int &xbase, const int &ybase);

    void insertUserMsgItem(const QString &name, const QString &ip);
    void insertUserMsgItem(const QString &, const int &);
    void sendFileMessage(const QString &from, const QString &ip);
    bool hasMessage() const;
    void show();

private:
    void setupUi();
    void freeUi();
    void setConnect();

signals:
    void readMessage(const QString &ip);

private slots:
    void showMessageWindow(QListWidgetItem*);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void closeEvent(QCloseEvent *);

private:
    QGridLayout* m_gridLayout;
    QListWidget *m_userMsgList;
    QTimer      *m_closeTimer;
    bool        m_isSendFile;
    QString     m_tips;

};

#endif // MESSAGE_BOX_H
