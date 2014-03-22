#include "transfer_file_window.h"
#include "send_file_treewidget.h"
#include "recv_file_treewidget.h"
#include "send_users_treewidget.h"
#include "send_history_treewidget.h"
#include "send_history_tableview.h"
#include "ipq.h"
#include "macai.h"
#include "preferences.h"
#include "send_message.h"
#include "message_thread.h"
#include "message.h"
#include "recv_file_thread.h"
#include "user_manager.h"
#include "owner.h"
#include "constants.h"
#include "system.h"
#include "useritemwidget.h"
#include "addrecieverdlg.h"
#include "send_file_propertise.h"

#include <QtGui>
#include <QSignalMapper>

TransferFileWindow::TransferFileWindow(QWidget *parent) :
    QWidget(parent)
{
    setupUi();

    m_sidebarBtnMapping = new QSignalMapper(this);
    connect(m_sidebarBtnMapping, SIGNAL(mapped(int)), stackedWidget, SLOT(setCurrentIndex(int)));
    connect(m_sidebarBtnMapping, SIGNAL(mapped(int)), this, SLOT(updateOperatorBtn(int)));
    setSidebarBtnMap();

//    connect(&(m_recvFileMap.m_timer), SIGNAL(timeout()), this, SLOT(updateTransferStatsInfo()));
    connect(addFileBtn, SIGNAL(clicked()), this, SLOT(addFile()));
    connect(startSendBtn, SIGNAL(clicked()), Macai::sendFileTreeWidget, SLOT(startSendSlot()));
    connect(reciveFileBtn, SIGNAL(clicked()), this, SLOT(save()));
//    connect(settingBtn, SIGNAL(clicked()), this, SLOT(testSharedFolder()));
    connect(addRecverBtn, SIGNAL(clicked()), this, SLOT(addRecverUser()));
    connect(delRecverBtn, SIGNAL(clicked()), this, SLOT(delRecverUser()));

}

TransferFileWindow::TransferFileWindow(Message message, QWidget *parent)
    : QWidget(parent), m_message(message), isCancelTransferCalled(false)
{
    setupUi();

    System::createSharedFolder();

    m_sidebarBtnMapping = new QSignalMapper(this);
    connect(m_sidebarBtnMapping, SIGNAL(mapped(int)), stackedWidget, SLOT(setCurrentIndex(int)));
    connect(m_sidebarBtnMapping, SIGNAL(mapped(int)), this, SLOT(updateOperatorBtn(int)));
    setSidebarBtnMap();

//    connect(&(m_recvFileMap.m_timer), SIGNAL(timeout()), this, SLOT(updateTransferStatsInfo()));
    connect(addFileBtn, SIGNAL(clicked()), this, SLOT(addFile()));
    connect(startSendBtn, SIGNAL(clicked()), Macai::sendFileTreeWidget, SLOT(startSendSlot()));
    connect(reciveFileBtn, SIGNAL(clicked()), this, SLOT(save()));
    connect(addRecverBtn, SIGNAL(clicked()), this, SLOT(addRecverUser()));
    connect(delRecverBtn, SIGNAL(clicked()), this, SLOT(delRecverUser()));
}

TransferFileWindow::~TransferFileWindow()
{
}

void TransferFileWindow::insertUserToList(const QString &name, const QString &ip)
{
//    m_recvUserMap.insert(ip, name);
//    QString label = name + "(" + ip +")";
    QTreeWidgetItem *item = new QTreeWidgetItem(Macai::sendUsersTreeWidget);
    item->setText(0, name);
    item->setText(1, ip);
//    m_reciversName.append(name);
//    m_reciversIp.append(ip);
}

void TransferFileWindow::clearUserList()
{
    Macai::sendUsersTreeWidget->clear();
}

void TransferFileWindow::collectHostDestinaton(const QString &ip, const QString &sharedNetName)
{
    // "//ip/IPQ_LoginName" IPQ_XXXX is a shared netName, but not a real shared directory.
    QString recvHostPath = "//"+ ip + "/" + AppNamePre + sharedNetName;
    m_recvHostPathList.append(recvHostPath);
}

QList<QString> TransferFileWindow::collectRecieverInfos()
{
    m_recvHostPathList.clear();
    m_recvUserMap.clear();
    m_reciversName.clear();
    m_reciversIp.clear();
    QTreeWidgetItemIterator it(Macai::sendUsersTreeWidget);
    while (*it)
    {
        QTreeWidgetItem *item = (*it);
        QString log_name = item->text(0);
        QString ip = item->text(1);
        QString recvHostPath = "//"+ ip + "/" + AppNamePre + log_name;
        m_recvHostPathList.append(recvHostPath);
        m_recvUserMap.insert(ip, log_name);
        m_reciversName.append(log_name);
        m_reciversIp.append(ip);

        ++it;
    }

    return m_recvHostPathList;
}

QList<QString> TransferFileWindow::reciversName() const
{
    return m_reciversName;
}

QList<QString> TransferFileWindow::reciversIp() const
{
    return m_reciversIp;
}

QMap<QString, QString> TransferFileWindow::reciversMap() const
{
    return m_recvUserMap;
}

void TransferFileWindow::closeEvent(QCloseEvent *e)
{
    m_recvUserMap.clear();
    emit setChecked(false);
    e->accept();
}

void TransferFileWindow::setupUi()
{

    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("transferFileWindow"));
    this->resize(865, 497);

    QGridLayout* gridLayout_4 = new QGridLayout(this);
    gridLayout_4->setContentsMargins(0, 0, 0, 0);
    gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
    QSplitter *splitter_0 = new QSplitter(this);
    splitter_0->setObjectName(QString::fromUtf8("splitter_0"));
    splitter_0->setOrientation(Qt::Horizontal);
    stackedWidget = new QStackedWidget(splitter_0);
    stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));

    QWidget *page_send = new QWidget();
    page_send->setObjectName(QString::fromUtf8("page_send"));
    QGridLayout* gridLayout_3 = new QGridLayout(page_send);
    gridLayout_3->setContentsMargins(0, 0, 0, 0);
    gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
//    fileTansferTreeWidget = new QTreeWidget(page_send);
    Macai::sendFileTreeWidget->setObjectName(QString::fromUtf8("transferFileTreeWidget"));
    Macai::sendFileTreeWidget->setParent(page_send);
    gridLayout_3->addWidget(Macai::sendFileTreeWidget, 0, 0, 1, 1);
    stackedWidget->addWidget(page_send);// page 1

    QWidget *page_recv = new QWidget();

    page_recv->setObjectName(QString::fromUtf8("page_recv"));
    QGridLayout* gridLayout_6 = new QGridLayout(page_recv);
    gridLayout_6->setContentsMargins(0, 0, 0, 0);
    gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
    Macai::recvFileTreeWidget->setObjectName(QString::fromUtf8("recvFileTreeWidget"));
    Macai::recvFileTreeWidget->setParent(page_recv);
    gridLayout_6->addWidget(Macai::recvFileTreeWidget, 0, 0, 1, 1);
    stackedWidget->addWidget(page_recv); // page 2

    QWidget *page_history = new QWidget();
    Macai::sendHistoryTableView->setParent(page_history);
    Macai::sendHistoryTableView->setObjectName(QString::fromUtf8("sendHistoryTableView"));
    page_history->setObjectName(QString::fromUtf8("page_history"));
    QGridLayout* gridLayout_5 = new QGridLayout(page_history);
    gridLayout_5->setContentsMargins(0, 0, 0, 0);
    gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
    gridLayout_5->addWidget(Macai::sendHistoryTableView, 0, 0, 1, 1);
    stackedWidget->addWidget(page_history); // page 3

    splitter_0->addWidget(stackedWidget);
    QSplitter *splitter_1 = new QSplitter(splitter_0);
    splitter_1->setObjectName(QString::fromUtf8("splitter_1"));
    splitter_1->setOrientation(Qt::Vertical);
//    splitter_1->setContentsMargins(0,0,0,0);

    QWidget *layoutWidget = new QWidget(splitter_1);
//    layoutWidget->setContentsMargins(0,0,0,0);
    QVBoxLayout *verticalLayout = new QVBoxLayout(layoutWidget);
    verticalLayout->setContentsMargins(0,9,0,9);
    verticalLayout->setSpacing(0);

    Macai::sendUsersTreeWidget->setParent(layoutWidget);
    Macai::sendUsersTreeWidget->setObjectName(QString::fromUtf8("sendUsersTreeWidget"));
    verticalLayout->addWidget(Macai::sendUsersTreeWidget);

    QHBoxLayout *hLayout_1 = new QHBoxLayout();
    addRecverBtn = new QPushButton(tr("add"), layoutWidget);
    addRecverBtn->setToolTip( tr("Add recieve file user.") );
    delRecverBtn = new QPushButton(tr("delete"), layoutWidget);
    addRecverBtn->setToolTip( tr("delete selected recieve file user from list.") );
    hLayout_1->addWidget(addRecverBtn);
    hLayout_1->addWidget(delRecverBtn);
    verticalLayout->addLayout(hLayout_1);
    splitter_1->addWidget(layoutWidget);

    Macai::sendFilePropertise->setObjectName(QString::fromUtf8("sendFilePropertise"));
    Macai::sendFilePropertise->setMinimumSize(QSize(0, 150));
    Macai::sendFilePropertise->setParent(splitter_1);
    splitter_1->addWidget(Macai::sendFilePropertise);
    splitter_0->addWidget(splitter_1);

    gridLayout_4->addWidget(splitter_0, 0, 1, 1, 1);

    sideBar = new QWidget(this);
    sideBar->setObjectName(QString::fromUtf8("sideBar"));
    sideBar->setMinimumSize(QSize(0, 0));
    sideBar->setMaximumSize(QSize(60, 16777215));
    QGridLayout* gridLayout_2 = new QGridLayout(sideBar);
    gridLayout_2->setSpacing(0);
    gridLayout_2->setContentsMargins(0, 0, 0, 0);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    fileSendBtn = new QToolButton(sideBar);
    fileSendBtn->setObjectName(QString::fromUtf8("fileSenderBtn"));
    sizePolicy.setHeightForWidth(fileSendBtn->sizePolicy().hasHeightForWidth());
    fileSendBtn->setSizePolicy(sizePolicy);
    fileSendBtn->setMinimumSize(QSize(0, 0));
    fileSendBtn->setMaximumSize(QSize(16777215, 50));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/images/file-send.png"), QSize(), QIcon::Normal, QIcon::Off);
    fileSendBtn->setIcon(icon1);
    fileSendBtn->setIconSize(QSize(32, 32));
    fileSendBtn->setCheckable(true);
    fileSendBtn->setChecked(true);
    fileSendBtn->setAutoRepeat(true);
    fileSendBtn->setAutoExclusive(true);
    fileSendBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    gridLayout->addWidget(fileSendBtn, 0, 0, 1, 1);

    fileRecvBtn = new QToolButton(sideBar);
    fileRecvBtn->setObjectName(QString::fromUtf8("fileRecvBtn"));
    sizePolicy.setHeightForWidth(fileRecvBtn->sizePolicy().hasHeightForWidth());
    fileRecvBtn->setSizePolicy(sizePolicy);
    fileRecvBtn->setMinimumSize(QSize(0, 0));
    fileRecvBtn->setMaximumSize(QSize(16777215, 50));
    QIcon icon3;
    icon3.addFile(QString::fromUtf8(":/images/file-recieve.png"), QSize(), QIcon::Normal, QIcon::Off);
    fileRecvBtn->setIcon(icon3);
    fileRecvBtn->setIconSize(QSize(32, 32));
    fileRecvBtn->setCheckable(true);
    fileRecvBtn->setAutoRepeat(true);
    fileRecvBtn->setAutoExclusive(true);
    fileRecvBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    gridLayout->addWidget(fileRecvBtn, 1, 0, 1, 1);

    fileHistoryBtn = new QToolButton(sideBar);
    fileHistoryBtn->setObjectName(QString::fromUtf8("fileHistoryBtn"));
    sizePolicy.setHeightForWidth(fileHistoryBtn->sizePolicy().hasHeightForWidth());
    fileHistoryBtn->setSizePolicy(sizePolicy);
    fileHistoryBtn->setMinimumSize(QSize(0, 0));
    fileHistoryBtn->setMaximumSize(QSize(16777215, 50));
    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/images/file-history.png"), QSize(), QIcon::Normal, QIcon::Off);
    fileHistoryBtn->setIcon(icon2);
    fileHistoryBtn->setIconSize(QSize(32, 32));
    fileHistoryBtn->setCheckable(true);
    fileHistoryBtn->setAutoRepeat(true);
    fileHistoryBtn->setAutoExclusive(true);
    fileHistoryBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    gridLayout->addWidget(fileHistoryBtn, 2, 0, 1, 1);



    QFrame *line = new QFrame(sideBar);
    line->setObjectName(QString::fromUtf8("line"));
    line->setMinimumSize(QSize(80, 0));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    gridLayout->addWidget(line, 4, 0, 1, 1);

    addFileBtn = new QToolButton(sideBar);
    addFileBtn->setObjectName(QString::fromUtf8("addFileBtn"));
    sizePolicy.setHeightForWidth(addFileBtn->sizePolicy().hasHeightForWidth());
    addFileBtn->setSizePolicy(sizePolicy);
    addFileBtn->setMinimumSize(QSize(0, 0));
    addFileBtn->setMaximumSize(QSize(16777215, 50));
    QIcon icon4;
    icon4.addFile(QString::fromUtf8(":/images/file-add.png"), QSize(), QIcon::Normal, QIcon::Off);
    addFileBtn->setIcon(icon4);
    addFileBtn->setIconSize(QSize(32, 32));
//    addFileBtn->setCheckable(true);
//    addFileBtn->setAutoRepeat(true);
//    addFileBtn->setAutoExclusive(true);
    addFileBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    gridLayout->addWidget(addFileBtn, 5, 0, 1, 1);

    startSendBtn = new QToolButton(sideBar);
    startSendBtn->setObjectName(QString::fromUtf8("exportLogBtn"));
    sizePolicy.setHeightForWidth(startSendBtn->sizePolicy().hasHeightForWidth());
    startSendBtn->setSizePolicy(sizePolicy);
    startSendBtn->setMinimumSize(QSize(0, 0));
    startSendBtn->setMaximumSize(QSize(16777215, 50));
    QIcon icon5;
    icon5.addFile(QString::fromUtf8(":/images/start-send.png"), QSize(), QIcon::Normal, QIcon::Off);
    startSendBtn->setIcon(icon5);
    startSendBtn->setIconSize(QSize(32, 32));
    startSendBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    gridLayout->addWidget(startSendBtn, 6, 0, 1, 1);

        reciveFileBtn = new QToolButton(sideBar);
        reciveFileBtn->setObjectName(QString::fromUtf8("reciveFileBtn"));
        sizePolicy.setHeightForWidth(reciveFileBtn->sizePolicy().hasHeightForWidth());
        reciveFileBtn->setSizePolicy(sizePolicy);
        reciveFileBtn->setMinimumSize(QSize(0, 0));
        reciveFileBtn->setMaximumSize(QSize(16777215, 50));
        QIcon icon5_1;
        icon5_1.addFile(QString::fromUtf8(":/images/start-send.png"), QSize(), QIcon::Normal, QIcon::Off);
        reciveFileBtn->setIcon(icon5_1);
        reciveFileBtn->setIconSize(QSize(32, 32));
        reciveFileBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        reciveFileBtn->hide();
        gridLayout->addWidget(reciveFileBtn, 7, 0, 1, 1);

    exportLogBtn = new QToolButton(sideBar);
    exportLogBtn->setObjectName(QString::fromUtf8("exportLogBtn"));
    sizePolicy.setHeightForWidth(exportLogBtn->sizePolicy().hasHeightForWidth());
    exportLogBtn->setSizePolicy(sizePolicy);
    exportLogBtn->setMinimumSize(QSize(0, 0));
    exportLogBtn->setMaximumSize(QSize(16777215, 50));
    QIcon icon6;
    icon6.addFile(QString::fromUtf8(":/images/file-export.png"), QSize(), QIcon::Normal, QIcon::Off);
    exportLogBtn->setIcon(icon6);
    exportLogBtn->setIconSize(QSize(32, 32));
//    exportLogBtn->setCheckable(true);
//    exportLogBtn->setAutoRepeat(true);
//    exportLogBtn->setAutoExclusive(true);
    exportLogBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    exportLogBtn->hide();
    gridLayout->addWidget(exportLogBtn, 8, 0, 1, 1);


    settingBtn = new QToolButton(sideBar);
    settingBtn->setObjectName(QString::fromUtf8("settingBtn"));
    settingBtn->setSizePolicy(sizePolicy);
    settingBtn->setMinimumSize(QSize(0, 0));
    settingBtn->setMaximumSize(QSize(16777215, 50));
    sizePolicy.setHeightForWidth(settingBtn->sizePolicy().hasHeightForWidth());
    settingBtn->setSizePolicy(sizePolicy);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/images/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
    settingBtn->setIcon(icon);
    settingBtn->setIconSize(QSize(32, 32));
//    settingBtn->setCheckable(true);
//    settingBtn->setAutoRepeat(true);
//    settingBtn->setAutoExclusive(true);
    settingBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    gridLayout->addWidget(settingBtn, 9, 0, 1, 1);

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

    gridLayout->addItem(verticalSpacer, 3, 0, 1, 1);


    gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


    gridLayout_4->addWidget(sideBar, 0, 0, 1, 1);


    retranslateUi();

    stackedWidget->setCurrentIndex(0);
    splitter_0->setStretchFactor(0, 70);
    splitter_0->setStretchFactor(1, 30);
    splitter_1->setStretchFactor(0, 60);
    splitter_1->setStretchFactor(1, 40);
}

void TransferFileWindow::freeUi()
{
}

void TransferFileWindow::retranslateUi()
{
    setWindowTitle(QApplication::translate("transferFileWindow", "Data Transfer", 0, QApplication::UnicodeUTF8));
    sideBar->setProperty("class", QVariant(QApplication::translate("transferFileWindow", "SideBar", 0, QApplication::UnicodeUTF8)));
    settingBtn->setText(QApplication::translate("transferFileWindow", "Setting", 0, QApplication::UnicodeUTF8));
    fileSendBtn->setText(QApplication::translate("transferFileWindow", "Sender", 0, QApplication::UnicodeUTF8));
    fileRecvBtn->setText(QApplication::translate("transferFileWindow", "Reciever", 0, QApplication::UnicodeUTF8));
    fileHistoryBtn->setText(QApplication::translate("transferFileWindow", "History", 0, QApplication::UnicodeUTF8));

    startSendBtn->setText(QApplication::translate("transferFileWindow", "Start", 0, QApplication::UnicodeUTF8));
    reciveFileBtn->setText(QApplication::translate("transferFileWindow", "Start", 0, QApplication::UnicodeUTF8));
    addFileBtn->setText(QApplication::translate("transferFileWindow", "Add File", 0, QApplication::UnicodeUTF8));
    exportLogBtn->setText(QApplication::translate("transferFileWindow", "Export", 0, QApplication::UnicodeUTF8));
}

void TransferFileWindow::setSidebarBtnMap()
{
    QList<QToolButton *> allButtons;
    allButtons << fileSendBtn << fileRecvBtn << fileHistoryBtn /*<< addFileBtn << exportLogBtn << addFileBtn*/;
    int index=0;
    foreach( QToolButton* obj, allButtons)
    {
        connect(obj, SIGNAL(clicked()), m_sidebarBtnMapping, SLOT(map()));
        m_sidebarBtnMapping->setMapping(obj, index);
        index++;
    }
}

QString TransferFileWindow::getSenderInfo()
{
    QString senderInfo;
    QString sender_name = m_message->owner().name();
    QString sender_group = m_message->owner().group();
    QString sender_ip = m_message->ip();

    senderInfo = sender_name + " (" + sender_group + " " + sender_ip +") ";

    return senderInfo;
}

void TransferFileWindow::addFile()
{
    Macai::sendFileTreeWidget->addFileClient();
}

void TransferFileWindow::addDir()
{
}

void TransferFileWindow::delFile()
{
}

void TransferFileWindow::sendFile()
{
    quint32 flags = 0;
    flags |= IPQ_SEND_FILE;

    emit messageReplyed();
}

void TransferFileWindow::updateOperatorBtn(int page)
{
    switch( page )
    {
    case 0:// send file view
        addFileBtn->show();
        startSendBtn->show();
        reciveFileBtn->hide();
        exportLogBtn->hide();
        break;
    case 1:
        addFileBtn->hide();
        startSendBtn->hide();
        reciveFileBtn->show();
        exportLogBtn->hide();
        Macai::recvFileTreeWidget->collectFiles();
        break;
    case 2:
        addFileBtn->hide();
        startSendBtn->hide();
        reciveFileBtn->hide();
        exportLogBtn->show();
        break;
    }
}

void TransferFileWindow::saveFile(QList<int> fileIdList, QString saveFileDir)
{

}

void TransferFileWindow::recvFileFinish()
{
    qDebug("MsgWindow::recvFileFinish");
}

void TransferFileWindow::recvFileError(QString errorString)
{
    qDebug() << "TransferFileWindow::recvFileError:" << errorString;
}

void TransferFileWindow::addRecverUser()
{
    addRecieverDlg = new AddRecieverDlg(this);
    addRecieverDlg->exec();
}

void TransferFileWindow::delRecverUser()
{
    QTreeWidgetItem *item = Macai::sendUsersTreeWidget->currentItem();
    if( item )
    {
        delete item;
    }
}

void TransferFileWindow::runRecvFileThread()
{

}

void TransferFileWindow::updateTransferStatsInfo()
{
}

void TransferFileWindow::save()
{
    QString directory = selectSaveDirectory();
    Macai::preferences->lastSaveFilePath = directory;
    int indexList = Macai::recvFileTreeWidget->count();
    QList<int> fileIdList;

    for(int i = 0; i < indexList; i++)
    {
        fileIdList << i;
    }
    saveFile(fileIdList, directory);
}

QString TransferFileWindow::selectSaveDirectory()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    QString directory = QFileDialog::getExistingDirectory(this,
            tr("Save File"),
            Macai::preferences->lastSaveFilePath,
            options);

    return directory;
}

void TransferFileWindow::setCurrentPage(int index)
{
    stackedWidget->setCurrentIndex(index);
    switch( index )
    {
    case 0:
        fileSendBtn->setChecked(true);
        break;
    case 1:
        fileRecvBtn->setChecked(true);
        break;
    case 2:
        fileHistoryBtn->setChecked(true);
        break;
    }
}
