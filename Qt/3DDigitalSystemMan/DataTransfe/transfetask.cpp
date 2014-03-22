#include "transfetask.h"
#include "dataclient.h"

#include <QMessageBox>
#include <QString>

TransfeTask::TransfeTask(QObject *parent) :
    QObject(parent), saveChanges(false)
{
}

bool TransfeTask::addClient(const QString &fileName, const QString &destinationFolder, const QByteArray &resumeState)
{
    foreach(DataJob job, jobs)
    {
        if (job.fileName == fileName && job.destinationDirectory == destinationFolder)
        {
            //QMessageBox::warning(this, tr("Already uploading"),tr("The data file %1 is already being uploaded.").arg(fileName));
            return false;
        }
    }

    // Create a new file client and attempt to parse the file data.
    DataClient *client = new DataClient(this);
    if (!client->setData(fileName))
    {
       // QMessageBox::warning(this, tr("Error"), tr("The torrent file %1 cannot not be opened/resumed.").arg(fileName),0, 0);
        delete client;
        return false;
    }
    client->setDestinationFolder(destinationFolder);
//    client->setDumpedState(resumeState);

    // 设置数据文件端状态更新的信号槽连接
    //...

    // Add the client to the list of downloading jobs.
    DataJob job;
    job.dataClient = client;
    job.fileName = fileName;
    job.destinationDirectory = destinationFolder;
    jobs << job;


    //这里把添加的文件数据显示到treeview中,
    //...

    //这里保存添加的数据到settings,为下次启动读取进度,以及添加到历史列表中

    //开始该文件数据的传输(或收当前任务类执行提交任务时再开始传输)
    //client->start();

    return true;
}

void TransfeTask::saveSettings()
{
    if (!saveChanges)
      return;
    saveChanges = false;

    // Prepare and reset the settings
    QSettings settings("Trolltech", "DataClients");
    settings.clear();

//    settings.setValue("LastDirectory", lastDirectory);
//    settings.setValue("UploadLimit", uploadLimitSlider->value());
//    settings.setValue("DownloadLimit", downloadLimitSlider->value());

    // Store data on all known torrents
    settings.beginWriteArray("DataClients");
    for (int i = 0; i < jobs.size(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("sourceFileName", jobs.at(i).fileName);
        settings.setValue("destinationFolder", jobs.at(i).destinationDirectory);
        settings.setValue("uploadedBytes", jobs.at(i).dataClient->uploadedBytes());
//        settings.setValue("resumeState", jobs.at(i).dataClient->dumpedState());
    }
    settings.endArray();
    settings.sync();
}

void TransfeTask::updateProgress(int percent)
{
    DataClient *client = qobject_cast<DataClient *>(sender());
    int row = rowOfClient(client);

    // Update the progressbar.
//    QTreeWidgetItem *item = torrentView->topLevelItem(row);
//    if (item)
    //        item->setText(2, QString::number(percent));
}

void TransfeTask::updateUploadRate(int bytesPerSecond)
{
    // Update the upload rate.
    DataClient *client = qobject_cast<DataClient *>(sender());
    int row = rowOfClient(client);
    QString num;
    num.sprintf("%.1f KB/s", bytesPerSecond / 1024.0);
//    torrentView->topLevelItem(row)->setText(4, num);

    if (!saveChanges)
    {
        saveChanges = true;
//        QTimer::singleShot(5000, this, SLOT(saveSettings()));
    }
}

int TransfeTask::rowOfClient(DataClient *client) const
{
    // Return the row that displays this client's status, or -1 if the
    // client is not known.
    int row = 0;
    foreach (DataJob job, jobs)
    {
        if (job.dataClient == client)
            return row;
        ++row;
    }
    return -1;
}
