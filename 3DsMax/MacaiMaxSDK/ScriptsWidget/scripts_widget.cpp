#include "scripts_widget.h"
#include "ui_scriptswidget.h"
#include "table_widget.h"
#include "constants.h"
#include "archivereader.h"

#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QIcon>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>

#include <QDebug>

ScriptsWidget::ScriptsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScriptsWidget)
{
    ui->setupUi(this);
    ui->toolBox->widget(0)->hide();
    ui->toolBox->setItemEnabled(0, false);
    ui->toolBox->removeItem(0);

    m_reader = new XmlReader(ui->toolBox);
    m_writer = new XmlWriter(ui->toolBox);

    m_xmlFile = QDir::currentPath() + "/" + XML_FILENAME;
//    if( !loadFile( m_xmlFile) )
    {
        reloadData();
    }
}

ScriptsWidget::~ScriptsWidget()
{
    saveFile(m_xmlFile);
    delete ui;
}

bool ScriptsWidget::loadFile(const QString &xmlfile)
{
    if (xmlfile.isEmpty())
        return false;

    QFile file(xmlfile);
    if ( !file.open(QFile::ReadOnly | QFile::Text ))
    {
        QMessageBox::warning(this, IDC_WIN_TITEL,
                             tr("Cannot load file %1\nError message:%2.\n\nWill setset to default data.")
                             .arg(xmlfile)
                             .arg(file.errorString()));
        return false;
    }

    if (!m_reader->read(&file))
    {
        QMessageBox::warning( this, IDC_WIN_TITEL,
                            tr("Parse error in file %1:\n\n%2")
                            .arg(xmlfile).arg( m_reader->errorString() ) );
        return false;
    }

    file.close();


    return true;
}

bool ScriptsWidget::saveFile(const QString &xmlFile)
{
    if (xmlFile.isEmpty())
        return false;

    QFile file(xmlFile);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Simple XML"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(xmlFile)
                             .arg( file.errorString() ));
        return false;
    }

    if ( !m_writer->write(&file) )
    {
        QMessageBox::warning(this, tr("Simple XML"),
                            tr("Save error in file %1:\n\n%2")
                            .arg(xmlFile).arg(file.errorString()));
        return false;
    }

    return true;
}

void ScriptsWidget::resetDefault()
{
    QFile file(XML_DEFAULT_FILE);
    if ( !file.open(QFile::ReadOnly | QFile::Text ))
    {
        qDebug() << "can not open file: " << file.fileName();
        return;
    }
    m_reader->read(&file);
    file.close();
}

void ScriptsWidget::reloadData()
{
    QString datapath = QDir::currentPath() + "/data";
//    QStringList datapathList;
//    datapathList << datapath + "/" + DATA_RENAME
//                 << datapath + "/" + DATA_MATERIAL
//                 << datapath + "/" + DATA_OBJEDIT
//                 << datapath + "/" + DATA_IMOREXPORT
//                 << datapath + "/" + DATA_SCENECHECK
//                 << datapath + "/" + DATA_OTHER;

    QDir dir( datapath );
    QFileInfoList fiList = dir.entryInfoList(QDir::Dirs | QDir::NoDotDot);
    int pageIconIdx = 0;
    foreach( QFileInfo fi, fiList )
    {
        if( pageIconIdx >= PageIcons.count() )
            pageIconIdx = pageIconIdx % PageIcons.count();
        listArchiveFiles(fi.filePath(), pageIconIdx);
        pageIconIdx++;
    }
}

void ScriptsWidget::listArchiveFiles()
{
    QString datapath = QDir::currentPath() + "/data";
    QDir dir( datapath );
    if( !dir.exists() )
    {
        QMessageBox::information(0, "File missing", tr("no this file or directory:%1").arg(datapath), 0);
        return;
    }

    QStringList filters;
    filters << ARCHIVE_TYPE_ZIP << ARCHIVE_TYPE_YRI << ARCHIVE_TYPE_STH;
    dir.setNameFilters(filters);
    QFileInfoList archives = dir.entryInfoList(QDir::Files);
    ArchiveReader archReader;
    int iconIndex = 0;
    foreach( QFileInfo archive, archives)
    {
        TableWidget *tablePage = new TableWidget;
        // TODO: 文件名包含包中的文件夹和文件夹内的文件(文件中带有文件夹路径)
        QStringList files = archReader.listFiles( archive.filePath(), ARCHIVE_PASSWORD );
        // TODO: 取任意一个文件,得到文件夹名,建立toolbox分类页面

        int idx = -1;
        if( files.count() > 1 )
            idx = files.at(1).lastIndexOf("/");
        else
            idx = files.at(0).lastIndexOf("/");
        if( idx != -1 )
        {
            QString pageTitle = files.at(0).left(idx) + QString(" (%1)").arg(files.count()-1);
            ui->toolBox->addItem(tablePage, QIcon(PageIcons.at(iconIndex)), pageTitle);
            files.removeAt(0);
        }
        else
        {
            QFileInfo file(archive);
            QString basename = file.baseName() + QString(" (%1)").arg(files.count()-1);
            ui->toolBox->addItem(tablePage, QIcon(PageIcons.at(iconIndex)), basename);
        }

        foreach( QString file, files )
        {
            // 只取文件名,去掉文件夹名
            int idx = file.lastIndexOf("/");    //idx==file.length()-1 是文件夹
            if (idx >= 0 && idx != file.length()-1)
            {
                file = file.right(file.length() - idx - 1);
            }
            if( idx == file.length()-1 )
                continue;

            QToolButton* toolbtn = tablePage->addButton( file, BtnIcon, archive.filePath() );
//            toolbtn->setToolTip();
        }

        iconIndex++;
        if( iconIndex >= PageIcons.count())
            iconIndex=0;
    }
}

void ScriptsWidget::listArchiveFiles(const QString &dir, int index)
{
    QDir d( dir );
    if( !d.exists() )
    {
        QMessageBox::information(0, "error", tr("no this file or directory:%1").arg(dir), 0);
        return;
    }

    QStringList filters;
    filters << ARCHIVE_TYPE_ZIP << ARCHIVE_TYPE_YRI << ARCHIVE_TYPE_STH;
    d.setNameFilters(filters);
    QFileInfoList archives = d.entryInfoList(QDir::Files);

    ArchiveReader archReader;
    foreach( QFileInfo archive, archives)
    {
        QStringList fileList = archReader.listFiles( archive.filePath(), ARCHIVE_PASSWORD );
        TableWidget *tablePage = new TableWidget;
        ui->toolBox->addItem(tablePage, QIcon(PageIcons.at(index)), d.dirName() );

        foreach( QString file, fileList )
        {
            // 只取文件名,去掉文件夹名
            int idx = file.lastIndexOf("/");    //idx==file.length()-1 是文件夹
            if (idx >= 0 && idx != file.length()-1)
            {
                file = file.right(file.length() - idx - 1);
            }
            if( idx == file.length()-1 )
                continue;

            QToolButton* toolbtn = tablePage->addButton( file, BtnIcon, archive.filePath() );
//            toolbtn->setToolTip();
        }
    }
}
