#include "send_history_tableview.h"
#include "send_file_treewidget.h"
#include "constants.h"
#include "system.h"
#include "macai.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QHeaderView>
#include <QMenu>
#include <QSqlRecord>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>

SendHistoryTableView::SendHistoryTableView(QWidget *parent) :
    QTableView(parent)
{
    setSelectionBehavior( QAbstractItemView::SelectRows ); //设置选择行为，以行为单位
    setSelectionMode( QAbstractItemView::SingleSelection );//设置选择模式，选择单行
//    this->setAlternatingRowColors(false);
    QHeaderView* vHeader = this->verticalHeader();
    vHeader->setDefaultSectionSize(20);
//    vHeader->setResizeMode(QHeaderView::Fixed);//固定列高,不可拖拉
    this->setFocusPolicy(Qt::NoFocus);
    sortByColumn(2, Qt::AscendingOrder);
    setEditTriggers ( QAbstractItemView::NoEditTriggers );

    if( !createConnectionDB() )
    {
        qDebug() << "can not connect database.";
        return;
    }

    m_model = new QSqlTableModel(this, m_db);
    initializeModel(m_model);

    //name
    resizeColumnToContents(0);
    //size
    resizeColumnToContents(1);
    //time
    resizeColumnToContents(2);
    //reciever
    resizeColumnToContents(3);
//    resizeColumnToContents(3);
    horizontalHeader()->setResizeMode(4, QHeaderView::Stretch);
    //state


    setConnect();
}

SendHistoryTableView::~SendHistoryTableView()
{
    m_db.close();
    m_db.removeDatabase(m_dbName);
}

void SendHistoryTableView::retranslateUi()
{
//    m_model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
//    m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Size"));
//    m_model->setHeaderData(2, Qt::Horizontal, QObject::tr("Time"));
//    m_model->setHeaderData(3, Qt::Horizontal, QObject::tr("Reviever"));
//    m_model->setHeaderData(4, Qt::Horizontal, QObject::tr("Project"));
//    m_model->setHeaderData(5, Qt::Horizontal, QObject::tr("State"));

    m_openAct->setText(tr("&Open"));
    m_deleteAct->setText(tr("&Delete"));
    m_refreshAct->setText(tr("&Refresh"));
}

bool SendHistoryTableView::createConnectionDB()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", SqlNameFileHistory);
    m_dbName = System::localIPQDocument() + SqlDatabaseName;
    m_db.setDatabaseName(m_dbName);
    if (!m_db.open())
    {
        QMessageBox::critical(0, tr("Cannot open database"),
                    tr("Unable to establish a database connection.\n"
                     "IPQ needs SQLite support. Please install "
                     "SQLite driver. \nClick Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    QStringList tableList = m_db.tables();
    QSqlQuery query(m_db);
    if(!tableList.contains(SqlTableFileHistory))
    {
<<<<<<< .mine
        QString createTable = QString("create table %1"
             "(filename varchar(64), size varchar(64), "
             "time varchar(64), reciever varchar(256), state varchar(64) )").arg(SqlTableFileHistory);
=======
        QString createTable = QString("create table %1"
            "(filename varchar(64), size varchar(64), "
             "time varchar(64), reciever varchar(256), project varchar(64), state varchar(64) )").arg(SqlTableFileHistory);
>>>>>>> .r96
        if ( !query.exec(createTable) )
        {
            QMessageBox::warning(this, tr("create table error"), tr("Error:")+query.lastError().text(), QMessageBox::Ok);
            return false;
        }
    }

    return true;
}

void SendHistoryTableView::initializeModel(QSqlTableModel *model)
{
    model->setTable(SqlTableFileHistory);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->removeColumn(0);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Size"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Time"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Reviever"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Project"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("State"));
    this->setModel(model);
}

void SendHistoryTableView::setConnect()
{
    connect(Macai::sendFileTreeWidget, SIGNAL(updateHistoryLog(QList<QString>)),
            this, SLOT(updateHistoryLogs(QList<QString>)) );
}

void SendHistoryTableView::contextMenuEvent(QContextMenuEvent */*event*/)
{
    QMenu *popMenu = new QMenu(this);
    m_openAct = popMenu->addAction(QIcon(":/images/file-open.png"),tr("&Open"));

    popMenu->addSeparator();
    m_deleteAct = popMenu->addAction(QIcon(":/images/file-delete.png"),tr("&Delete"));
    popMenu->addSeparator();
    m_refreshAct = popMenu->addAction(QIcon(":/images/file-refresh.png"),tr("&Refresh"));

    // 需要减去表头高度
    int row = this->rowAt( mapFromGlobal(QCursor::pos()).y()-this->horizontalHeader()->height() );
    if( row == -1)
    {
        this->clearSelection();;
        m_openAct->setEnabled(false);
        m_deleteAct->setEnabled(false);
    }
    else
    {
        m_openAct->setEnabled(true);
        m_deleteAct->setEnabled(true);
    }

    connect(m_openAct, SIGNAL( triggered() ), this, SLOT( openFile() ));
    connect(m_deleteAct, SIGNAL( triggered() ), this, SLOT( deleteFile() ));
    connect(m_refreshAct, SIGNAL( triggered() ), this, SLOT( refresh() ));
    popMenu->exec(cursor().pos());
}

void SendHistoryTableView::insertTable(const QList<QString> &logs)
{
<<<<<<< .mine
//    QString filename = logs.at(0);
//    QString filesize = logs.at(1);
//    QString time = logs.at(2);
//    QString recver = logs.at(3);
//    QString state = logs.at(4);
=======
//    QString filename = logs.at(0);
//    QString filesize = logs.at(1);
//    QString time = logs.at(2);
//    QString recver = logs.at(3);
//    QString project = logs.at(4);
//    QString state = logs.at(5);
>>>>>>> .r96
    int row = m_model->rowCount();
    m_model->insertRow(row);
    for(int i = 0; i < logs.count(); i++)
    {
        QString strValue = logs.at(i);
        m_model->setData(m_model->index(row, i), strValue);  //给新行添加属性值,字段在第i列上
    }
    m_model->submitAll(); //直接提交
}

void SendHistoryTableView::updateHistoryLogs(const QList<QString> &logs)
{
    insertTable(logs);
}

void SendHistoryTableView::openFile()
{
    int curRow = this->currentIndex().row();
    if( curRow != -1 )
    {

        QString filename = m_model->index(curRow, 0).data().toString();
        QFileInfo fileInfo(filename);
        QDesktopServices::openUrl( QUrl::fromLocalFile(fileInfo.absolutePath()) );
//        QProcess *process = new QProcess(this);
//        QString program = "explorer.exe ";
//        QString nativeName = dir.toNativeSeparators(dir.absolutePath());
//        process->startDetached( program, QStringList() << fileInfo.absolutePath() );
    }
}

void SendHistoryTableView::deleteFile()
{
    int curRow = this->currentIndex().row();
/*
    qDebug() << curRow;
    QSqlQuery query(m_db);
    query.exec( QString("select * from %1;").arg(SqlTableFileHistory) );
    QSqlRecord recd = query.record();
    int fieldCount = recd.count();
    qDebug() <<fieldCount;
    while( query.next() )
    {
        qDebug() << query.value(0).toString() << query.value(1).toString() << query.value(2).toString() << query.value(3).toString() << query.value(4).toString() << query.value(5).toString();
    }
*/
    if( curRow != -1 )
    {
        m_model->removeRow(curRow);   //删除一行
        m_model->submitAll();
    }
}

void SendHistoryTableView::refresh()
{
    m_model->select();
}



