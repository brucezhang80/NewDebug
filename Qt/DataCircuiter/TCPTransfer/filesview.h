#ifndef FILESVIEW_H
#define FILESVIEW_H

#include <QApplication>
#include <QTreeWidget>
#include <QMouseEvent>
#include <QItemDelegate>
#include <QDir>
#include <QApplication>
#include "qtfilecopier.h"
#include "dataclient.h"

class FilesView : public QTreeWidget
{
    Q_OBJECT
public:
    explicit FilesView(QWidget *parent = 0);
    ~FilesView();
    const DataClient *clientForRow(int row) const;
    void getFolderSize(const QString & path, qint64 &size);
    //void getFolderSize(const QDir & dir, qint64 &size);
    void bytesToSize(const qint64 & bytes, QString & size);
    void setServerPath(const QString &path);

protected:
    //void mouseMoveEvent(QMouseEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
private:
    int rowOfClient(DataClient *client) const;
    bool addClient(const QString &fileName, const QString &destinationFolder,
                    const QByteArray &resumeState = QByteArray());

    QtFileCopier::CopyFlags currentFlags() const;

    QString createdDir;
    QString copiedDir;
    QString movedDir;
    QString copiedFiles;
    QString movedFiles;

    QString serverPath;

    int curRuning;
    int completedCounts;

    struct DataJob
    {
        QtFileCopier *copier;
        DataClient *dataClient;
        QString fileName;
        QString destinationDirectory;
    };

    QTimer *timer;
    QList<DataJob> jobs;

signals:
    void updateControlSignal(bool bEnabled);

public slots:
    void stateChanged(QtFileCopier::State state);
    void startUploadDataClientSlot();
    void stopUploadDataClientSlot();

    void updateProgressSlot(int percent);
    void updateUploadRateSlot(QString);
    void updateTransferStateSlot(QString);
    void updateCompletedSlot(QString);
    void updateRemainTimeSlot(QString);
    void updateTotalTimeSlot(QString);
    void transferDoneSlot(DataClient*);
    void transferNextSlot();
    void transferAllDone();
    void clearDataJobs();
    void clearAllClient();

    void currentProjectPath(int);
};

// TorrentViewDelegate is used to draw the progress bars.
class FilesViewDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    inline FilesViewDelegate(QWidget *parent) : QItemDelegate(parent) {}

    inline void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
    {
        if (index.column() != 2) {
            QItemDelegate::paint(painter, option, index);
            return;
        }

        // Set up a QStyleOptionProgressBar to precisely mimic the
        // environment of a progress bar.
        QStyleOptionProgressBar progressBarOption;
        progressBarOption.state = QStyle::State_Enabled;
        progressBarOption.direction = QApplication::layoutDirection();
        progressBarOption.rect = option.rect;
        progressBarOption.fontMetrics = QApplication::fontMetrics();
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.textAlignment = Qt::AlignCenter;
        progressBarOption.textVisible = true;

        // Set the progress and text values of the style option.
        int progress = qobject_cast<FilesView *>(parent())->clientForRow(index.row())->d_ptr->completedPercent;
        progressBarOption.progress = progress < 0 ? 0 : progress;
        progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);

        // Draw the progress bar onto the view.
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    }
};

#endif // FILESVIEW_H
