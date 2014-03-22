#ifndef TRANSFETASK_H
#define TRANSFETASK_H

#include <QtCore>
#include <QtGui>
#include <QObject>

class DataClient;

class TransfeTask : public QObject
{
    Q_OBJECT
public:
    explicit TransfeTask(QObject *parent = 0);
    const DataClient *dataClientForRow(int row) const;

signals:
    
public slots:
    //void addClient();

private:
    int rowOfClient(DataClient *client) const;
    bool addClient(const QString &fileName, const QString &destinationFolder,
                    const QByteArray &resumeState = QByteArray());
    void saveSettings();

    void updateProgress(int percent);
    void updateUploadRate(int bytesPerSecond);


    struct DataJob
    {
        DataClient *dataClient;
        QString fileName;
        QString destinationDirectory;
    };

    QList<DataJob> jobs;

    bool saveChanges;
};

#endif // TRANSFETASK_H
