#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "HttpDownload.h"
#include "ParseHtml.h"

#include <QMovie>
//#include <QThread>

#include <QDebug>

QString formatSize(qint64 size)
{
    QString str = QString("%1 B").arg(size);
    int base = 1024;
    qint64 mult = 1;
    for (int i = 0; i < 4; i++)
    {
        qint64 div = size / mult;
        if (div < base)
        {
            double val = (double)size / mult;
            switch (i)
            {
                case 1: str = QString("%1Kb").arg(val, 0, 'f', 1); break;
                case 2: str = QString("%1Mb").arg(val, 0, 'f', 1); break;
                case 3: str = QString("%1Gb").arg(val, 0, 'f', 1); break;
                default: break;
            }
            break;
        }
        mult *= base;
    }
    return str;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
//    m_parseHtml(new ParseHtml),
    m_httpDownload(new HttpDownload())
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);

//    QThread* thread = new QThread;
//    m_httpDownload->moveToThread(thread);
//    thread->start();

    connect(ui->downloadBtn, SIGNAL(clicked()),
            this, SLOT(startDownload()));

    connect(m_httpDownload, SIGNAL(updateDownloadProgressbar(qint64, qint64)),
            this, SLOT(updateProgressbar(qint64, qint64)));
    connect(m_httpDownload, SIGNAL(outputLog(QString)),
            this, SLOT(appendLog(QString)));
    connect(m_httpDownload, SIGNAL(htmlCount(int)),
            this, SLOT(updateHtmlCount(int)));

    connect(m_httpDownload, SIGNAL(imgCount(int)),
            this, SLOT(updateImgCount(int)));

//    connect(m_parseHtml, SIGNAL(updateLog(QStringList)),
//                this, SLOT(appendLog(QStringList)));

//    connect(m_parseHtml, SIGNAL(downloadImg(QString)),
//                this, SLOT(downloadImg(QString)));
//    connect(m_parseHtml, SIGNAL(updateCount(int)),
//                this, SLOT(updateCount(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateProgressbar(qint64 completedBytes, qint64 totalBytes)
{
    ui->progressBar->setMaximum(totalBytes);    //最大值
    ui->progressBar->setValue(completedBytes);  //当前值

    // calculate the download speed
    double speed = completedBytes * 1000.0 / m_downloadTime.elapsed();
    QString unit;
    if (speed < 1024)
    {
        unit = "bytes/sec";
    }
    else if (speed < 1024*1024)
    {
        speed = qMax(speed/1024, 1.0);
        unit = "KB/s";
    }
    else
    {
        speed = qMax(speed/(1024*1024), 1.0);
        unit = "MB/s";
    }

    ui->speedLabel->setText(QString::fromLatin1("%1 %2")
                           .arg(speed, 3, 'f', 1).arg(unit) );

    QString lessByte = QString("%1/%2")
            .arg(formatSize(completedBytes))
            .arg(formatSize(totalBytes));
    ui->sizeLabel->setText(lessByte);
}

void MainWindow::startDownload()
{
<<<<<<< .mine
    m_downloadTime.start();
=======
>>>>>>> .r170
    const QString url = ui->urlEdt->text();
<<<<<<< .mine
    m_httpDownload->append( QUrl(url) );
    m_httpDownload->download();

//    m_parseHtml->append(QUrl(url));
//    qDebug() << "startDownload";
=======
//    m_httpDownload->append( QUrl(url) );
    m_httpDownload->download(QUrl(url));
>>>>>>> .r170
}

void MainWindow::appendLog(const QString &log)
{
    m_downloadTime.start();
    ui->logEdt->append(log);
}

void MainWindow::appendLogs(const QStringList &logs)
{
    foreach (QString log, logs)
    {
        ui->logEdt->append(log);
    }
    m_httpDownload->append( logs );
//    m_httpDownload->download();
}

void MainWindow::updateHtmlCount(int count)
{
    QString srtCount = QString("%1 html(s)").arg(count);
    ui->htmlCountLabel->setText(srtCount);
}

void MainWindow::updateImgCount(int count)
{
    QString srtCount = QString("%1 image(s)").arg(count);
    ui->imgCountLabel->setText(srtCount);

}
