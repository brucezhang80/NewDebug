#ifndef TRANSFER_FILE_WINDOW_H
#define TRANSFER_FILE_WINDOW_H

#include <QWidget>
#include <QMap>

#include "owner.h"
#include "message.h"

class QGridLayout;
class QSplitter;
class QStackedWidget;
class QTreeWidget;
class QListWidget;
class QLabel;
class QToolButton;
class QFrame;
class QSpacerItem;
class QSignalMapper;
class QPushButton;

class SendFileMap;
class RecvFileThread;
class AddRecieverDlg;

class TransferFileWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TransferFileWindow(QWidget *parent = 0);
    TransferFileWindow(Message message, QWidget *parent = 0);
    ~TransferFileWindow();
    void retranslateUi();

    void insertUserToList(const QString &name, const QString &ip);
    void clearUserList();
    void collectHostDestinaton(const QString &ip, const QString &sharedNetName);
    QList<QString> collectRecieverInfos();
    QList<QString> reciversName() const;
    QList<QString> reciversIp() const;
    QMap<QString, QString> reciversMap() const;

    void insertFileClient();
//    void initRecvFileInfo();

    void setMessage(Message msg){ m_message = msg; }
    void setCurrentPage(int index);

protected:
    void closeEvent(QCloseEvent *);

signals:
    void setChecked(bool);
    void addFileFinished();
    void messageReplyed();

    void stopTransfer();
    void abortTransfer();

public slots:
//    void show(){ qDebug() << "???????????"; QWidget::show(); }

private slots:
    void addFile();
    void addDir();
    void delFile();
    void sendFile();
    void updateOperatorBtn(int page);

    void saveFile(QList<int> fileIdList, QString saveFileDir);
    void recvFileFinish();
    void recvFileError(QString errorString);
    void addRecverUser();
    void delRecverUser();

//    void updateFileCount();
//    void cancelTransfer();
    void runRecvFileThread();
    void updateTransferStatsInfo();
//    void retryRecvFile();

    void save();
    QString selectSaveDirectory();

private:
    void setupUi();
    void freeUi();
    void setSidebarBtnMap();

    QString getSenderInfo();
//    void retryTransfer();

//    bool hasRecvFile () const;

//    void updateConnections();
//    void resetConnections();

//    void removeRecvOkFile();

    Message                 m_message;
    SendFileMap             *m_sendFileMap;
    RecvFileThread          *m_recvFileThread;
    QMap<QString, QString>  m_recvUserMap;
    QList<QString>          m_recvHostPathList;
    QList<QString>          m_reciversName;
    QList<QString>          m_reciversIp;

    bool                    isCancelTransferCalled;

    QSignalMapper           *m_sidebarBtnMapping;

    QPushButton             *addRecverBtn;
    QPushButton             *delRecverBtn;

    QStackedWidget          *stackedWidget;
    QWidget                 *sideBar;

    QToolButton *fileSendBtn;
    QToolButton *fileHistoryBtn;
    QToolButton *fileRecvBtn;

    QToolButton *addFileBtn;
    QToolButton *startSendBtn;
    QToolButton *reciveFileBtn;
    QToolButton *exportLogBtn;
    QToolButton *settingBtn;

    AddRecieverDlg *addRecieverDlg;
};

#endif // TRANSFER_FILE_WINDOW_H
