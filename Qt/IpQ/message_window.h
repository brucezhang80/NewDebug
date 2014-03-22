#ifndef MESSAGE_WINDOW_H
#define MESSAGE_WINDOW_H

#include <QWidget>
#include <QTextTableFormat>
#include <QCloseEvent>
#include "message.h"

class QGridLayout;
class QSplitter;
class QTextEdit;
class QPushButton;
class QToolButton;
class QSpacerItem;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QTreeWidget;

class MessageWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MessageWindow(QWidget *parent = 0);
    explicit MessageWindow(Message msg, QWidget *parent = 0);
    explicit MessageWindow(QString hostName, QString ip, QString group, QWidget *parent = 0);

    ~MessageWindow();

    QString hostName() const { return m_hostName;}
    QString ip()const { return m_ip; }
    void setTheme(const QString &themeFile);

signals:
    void removeMessageWindow();

private slots:
    void setEnterHotkey();
    void setCtrlEnterHotkey();

public slots:
    void sendMessage();
    void recvMessage(Message msg);

protected:
    void closeEvent(QCloseEvent *);
    bool eventFilter(QObject *obj, QEvent *ev);

private:
    void setupUi();
    void freeUi();
    void retranslateUi();
    void setConnect();
    void appendMessage(const QString &from, const QString &msg);

    void setUserInfo(const QString &name, const QString &hostnaem, const QString &ip, const QString &group);
    void setTheme();

    void createSendPopmenu();

    Message     m_message;
    QString     m_hostName;
    QString     m_ip;
    QString     m_group;
    QTextTableFormat    m_tableFormat;

private:
    QGridLayout *gridLayout;
    QSplitter   *splitter_3;
    QSplitter   *splitter;
    QWidget     *widget;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
//    QSplitter   *splitter_2;
    QWidget     *widget1;
    QVBoxLayout *verticalLayout_2;

    QToolButton *btn_msgShaking;
    QToolButton *btn_msgSetFont;
    QToolButton *btn_msgSendFile;
    QToolButton *btn_msgSendImg;
    QTextEdit   *tedt_msgOutputText;
    QTextEdit   *tedt_msgInputText;
    QToolButton *btn_msgSendMsg;
//    QToolButton *btn_hotKey;
    QPushButton *btn_msgClose;
    QLabel      *userInfoLabel;
    QTreeWidget *fileTransferTree;


    QAction     *m_enterAct;
    QAction     *m_ctrlEnterAct;
};

#endif // MESSAGE_WINDOW_H
