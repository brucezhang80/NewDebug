#include "fileclient.h"
#include "filetransfer.h"
#include <QtGui>

/* @�����ļ���С��byteת�������ü�����λKb,Mb,Gb
 * @������ bytesToSize
 * @���� bytes
 * @����ֵ QString
 */
static QString bytesToSize(const qint64 &bytes)
{
    QString formatSize;
    qint32 B = 1; //byte
    qint32 KB = 1024 * B; //kilobyte
    qint32 MB = 1024 * KB; //megabyte
    qint64 GB = 1024 * MB; //gigabyte
    qint64 TB = 1024 * GB;//terabyte

    if ((bytes >= KB) && ( bytes < MB))
    {
        formatSize = QString("%1 KB").arg( (double)bytes / KB, 0, 'f', 1 );
    }
    else if ((bytes >= MB) && (bytes < GB))
    {
        formatSize = QString("%1 MB").arg( (double)bytes / MB, 0, 'f', 1 );
    }
    else if ((bytes >= GB) && (bytes < TB))
    {
       formatSize = QString("%1 GB").arg( (double)bytes / GB, 0, 'f', 1 );
    }
    else if (bytes >= TB)
    {
        formatSize = QString("%1 TB").arg( (double)bytes / TB, 0, 'f', 1 );
    }
    else
    {
        formatSize = QString("%1 B").arg((double)bytes, 0, 'f', 1);
    }

    return formatSize;
}

/* @�õ�һ���ļ����ڵ������ļ���С,���ļ����ܴ�С
 * @������ getFolderSize
 * @���� path �ļ���·��
 * @����ֵ qint64 �ļ��д�Сbyte
 */
static qint64 getFolderSize(const QString & path )
{
    QDir *dir=new QDir(path);
    QStringList filter;
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>( dir->entryInfoList(filter) );
    qint64 _size = 0;
    for ( int i = 0; i < fileInfo->size(); ++i )
    {
        _size +=  fileInfo->at(i).size();
    }


    return _size;
}


/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
FileTransfer::FileTransfer(QWidget *parent) :
    QTreeWidget(parent)
{
    QStringList headers;
    headers << tr("#") << tr("File") << tr("Progress") << tr("Size") << tr("Rate");
    this->setHeaderLabels(headers);

    QHeaderView *header = this->header();
    header->resizeSection(0, 20);
    header->resizeSection(1, 120);
    header->resizeSection(2, 100);
    header->resizeSection(3, 100);
    //header->resizeSection(4, 60);

    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setAlternatingRowColors(true);
    this->setRootIsDecorated(false);
    this->setAcceptDrops(true);

    this->setItemDelegate(new FileTransferDelegate(this));
}

FileTransfer::~FileTransfer()
{
    jobs.clear();
}

/* @�õ�treewidgetһ�е�����,ÿһ������Ϊһ��FileClient����
 * @������ FileTransfer::clientForRow
 * @���� row
 * @����ֵ FileClient
 */
const FileClient *FileTransfer::clientForRow(int row) const
{
    return jobs.at(row).fileClient;
}

QList<FileTransfer::FileJob> FileTransfer::fileJobs()
{
    return jobs;
}

void FileTransfer::dragMoveEvent(QDragMoveEvent *event)
{
    if( event->mimeData()->hasUrls() )
    {
        event->acceptProposedAction();
    }
    else
        event->ignore();
}

void FileTransfer::dropEvent(QDropEvent *event)
{
    if( event->mimeData()->hasFormat("text/uri-list") )
    {
        QString localFile;
        foreach(QUrl url, event->mimeData()->urls())
        {
            localFile = url.toLocalFile();
            event->acceptProposedAction();
            event->setDropAction(Qt::CopyAction);
            addFile(localFile);
        }
    }
    else
        event->ignore();
}

void FileTransfer::dragEnterEvent(QDragEnterEvent *event)
{
    if( event->mimeData()->hasUrls() )
    {
        event->acceptProposedAction();
    }
}

/* @����һ���ļ�
 * @������ FileTransfer::addFile
 * @���� filename
 * @����ֵ ��
 */
bool FileTransfer::addFile(const QString &filename)
{
    foreach(FileJob job, jobs)
    {
        if (job.fileName == filename)
        {
            QMessageBox::warning(this, tr("Already uploading"),tr("The data file %1 is already being uploaded.").arg(filename));
            return false;
        }
    }
    FileClient *client = new FileClient(this);
    // ���һ���ļ�����ǰ�����б�
    FileJob job;
    job.fileClient = client;
    job.fileName = filename;
    jobs << job;

    // ���������ļ���״̬���µ��źŲ�����
    //...

    QFileInfo fileinfo = QFileInfo(filename);
    QString strSize;
    if( fileinfo.isFile() )
    {

        strSize = bytesToSize( fileinfo.size() );
    }
    else
    {
        qint64 _size=0;
        _size = getFolderSize( filename );
        strSize = bytesToSize( _size );
    }

    QFileIconProvider fileIcon;
    QIcon icon = fileIcon.icon(fileinfo);

    QTreeWidgetItem *item = new QTreeWidgetItem(this);
    QString baseFileName = QFileInfo(filename).fileName();
    // # file progress size rate
    item->setText(0, QString("%1").arg( this->model()->rowCount()) );
    item->setText(1, baseFileName); item->setIcon(1, icon);
    item->setText(2, "0%");
    item->setText(3, strSize);
    item->setText(4, "0.0 KB/s");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);

    return true;
}
