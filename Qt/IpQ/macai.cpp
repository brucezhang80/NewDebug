#include <QSettings>
#include <QIcon>
#include <QtGui>
#include <QTranslator>

#include "system.h"
#include "macai.h"
#include "constants.h"
#include "main_window.h"
#include "aboutdialog.h"
#include "systemtray.h"
#include "usertreewidget.h"
#include "user_manager.h"
#include "message_thread.h"
#include "preferences.h"

#include "window_manager.h"
#include "message_box.h"
#include "transfer_file_window.h"
#include "send_file_treewidget.h"
#include "recv_file_treewidget.h"
#include "file_tcpserver.h"
#include "send_users_treewidget.h"
#include "send_history_treewidget.h"
#include "send_history_tableview.h"
#include "send_file_propertise.h"
#include "system_settings.h"
#include "configuration_page.h"
#include "messageconfig_page.h"
#include "folderconfig_page.h"
#include "helperconfig_page.h"

using namespace Macai;

QTranslator           *Macai::translator = 0;
MainWindow            *Macai::mainWindow;
AboutDialog           *Macai::aboutDialog = 0;
QSettings             *Macai::settings = 0;
UserManager           *Macai::userManager = 0;
MessageThread         *Macai::messageThread = 0;
SystemTray            *Macai::systemTray = 0;
QMap<QString, QIcon*>  Macai::iconSet;
Preferences           *Macai::preferences = 0;
UserTreeWidget        *Macai::userTreeWidget = 0;
WindowManager         *Macai::windowManager = 0;
MessageBox            *Macai::messageBox = 0;
TransferFileWindow    *Macai::transferFileWindow = 0;
SendFileTreeWidget    *Macai::sendFileTreeWidget = 0;
RecvFileTreeWidget    *Macai::recvFileTreeWidget = 0;
FileTcpServer         *Macai::fileTcpServer = 0;
SendUsersTreeWidget   *Macai::sendUsersTreeWidget = 0;
//SendHistoryTreeWidget *Macai::sendHistoryTreeWidget = 0;
SendHistoryTableView  *Macai::sendHistoryTableView = 0;
SendFilePropertise    *Macai::sendFilePropertise = 0;
SystemSettings        *Macai::systemSettings = 0;
ConfigurationPage     *Macai::configurationPage = 0;
MessageConfigPage     *Macai::messageConfigPage = 0;
FolderConfigPage      *Macai::folderConfigPage = 0;
HelperConfigPage      *Macai::helperConfigPage = 0;

static void createIconSet()
{
    iconSet.insert("normal", new QIcon(QString(":/icons/") + "qipmsg.xpm"));
    iconSet.insert("receive",new QIcon(QString(":/icons/") + "qipmsg_recv.xpm"));

}

void Macai::MacaiStart(QString path)
{
//    translator = new QTranslator(qApp);
//    createIconSet();
    if (path.isEmpty())
    {
        settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                 QString(Company), QString(Program));
    }
    else
    {
        QString fileName = path + "/ipq.ini";
        settings = new QSettings(fileName, QSettings::IniFormat);
    }

    if( System::createSharedFolder() )
    {
        qDebug() << "Create shared folder failed.";
    }

    preferences = new Preferences;
    aboutDialog = new AboutDialog(mainWindow);
    userTreeWidget = new UserTreeWidget;
    windowManager = new WindowManager;
    messageThread = new MessageThread;
    fileTcpServer = new FileTcpServer;
    userManager = new UserManager;
    mainWindow = new MainWindow;
    systemTray = new SystemTray;
    Macai::systemTray->create();
    messageBox = new MessageBox;

    sendFileTreeWidget = new SendFileTreeWidget;
    recvFileTreeWidget = new RecvFileTreeWidget;
    sendUsersTreeWidget = new SendUsersTreeWidget;
//    sendHistoryTreeWidget = new SendHistoryTreeWidget;
    sendHistoryTableView = new SendHistoryTableView;
    sendFilePropertise = new SendFilePropertise;
    transferFileWindow = new TransferFileWindow;

    configurationPage = new ConfigurationPage;
    messageConfigPage = new MessageConfigPage;
    folderConfigPage = new FolderConfigPage;
    helperConfigPage = new HelperConfigPage;
    systemSettings = new SystemSettings;

    setThemeAll(DefualtTheme);
}

void Macai::MacaiEnd()
{
    qDebug("Macai::MacaiEnd");

    delete userManager;

    delete windowManager;

    delete systemTray;

    delete messageThread;

    delete sendFileTreeWidget;
    delete recvFileTreeWidget;
    delete sendUsersTreeWidget;
//    delete sendHistoryTreeWidget;
    delete sendHistoryTableView;
    delete sendFilePropertise;
    delete transferFileWindow;

    delete fileTcpServer;

    delete preferences;

    delete settings;

    delete translator;
}

void Macai::setThemeAll(const QString &themeFile)
{
    QFile qss(themeFile);
    if( !qss.open(QFile::ReadOnly) )
    {
        QMessageBox::information(0, QObject::tr("Set theme"),
                                QObject::tr("Set theme failed: Theme file"
                                "is missing or read error."), QMessageBox::Ok);
        return;
    }

    QByteArray data = qss.readAll();

    mainWindow->setStyleSheet(data);
    aboutDialog->setStyleSheet(data);
    messageBox->setStyleSheet(data);
    transferFileWindow->setStyleSheet(data);
    systemSettings->setStyleSheet(data);

    qss.close();
}

void Macai::setThemeBkColor(const QColor &color)
{
    QString qssSytle = QString("QWidget{background-color:%1;color:rgba(255,255,255,255);}").arg(color.name());
    mainWindow->setStyleSheet(qssSytle);
    aboutDialog->setStyleSheet(qssSytle);
    messageBox->setStyleSheet(qssSytle);
    transferFileWindow->setStyleSheet(qssSytle);
    systemSettings->setStyleSheet(qssSytle);
}

void Macai::retranslateAllUi(const QString &langFile)
{

    QTranslator *lang = new QTranslator();
    QFileInfo file(langFile);
    if( !lang->load(langFile, ".") )
        qDebug() << "changing language failed:" << langFile;
    qApp->installTranslator(lang);
    aboutDialog->retranslateUi();
//    userTreeWidget->retranslateUi();
//    windowManager->retranslateUi();
//    userManager->retranslateUi();
    mainWindow->retranslateUi();
    systemTray->retranslateUi();
    messageBox->retranslateUi();

    sendFileTreeWidget->retranslateUi();
    sendUsersTreeWidget->retranslateUi();
//    sendHistoryTableView->retranslateUi();
    sendFilePropertise->retranslateUi();
    transferFileWindow->retranslateUi();

    configurationPage->retranslateUi();
    messageConfigPage->retranslateUi();
    folderConfigPage->retranslateUi();
    helperConfigPage->retranslateUi();
    systemSettings->retranslateUi();
}

QString Macai::fileCountString(int i)
{
    QString s;
    if (i <= 1)
    {
        s = QObject::tr("%1 file").arg(i);
    }
    else
    {
        s = QObject::tr("%1 files").arg(i);
    }

    return s;
}

QPoint Macai::randomNearMiddlePoint()
{
    QRect rect(QApplication::desktop()->screenGeometry());
    int x = qrand() % (rect.width() / 4);
    int y = qrand() % (rect.height() / 6);
    return QPoint(rect.width()/6 + x, rect.height()/8 + y);

}
