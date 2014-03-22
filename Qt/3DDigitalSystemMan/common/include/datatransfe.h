#ifndef DATATRANSFE_H
#define DATATRANSFE_H

#include <QtCore\qglobal.h>
#if defined(DATATRANSFE_LIBRARY)
#  define DATATRANSFESHARED_EXPORT Q_DECL_EXPORT
#else
#  define DATATRANSFESHARED_EXPORT Q_DECL_IMPORT
#endif
#include "dataclient.h"

#include <QWidget>
#include <QTreeWidget>
#include <QMouseEvent>
#include <QItemDelegate>
#include <QApplication>

QT_BEGIN_NAMESPACE
class QDir;
class QItemDelegate;
QT_END_NAMESPACE

//class TransfeTask;
class DataClient;

class DATATRANSFESHARED_EXPORT DataTransfe : public QTreeWidget
{
    Q_OBJECT
public:
    explicit DataTransfe(QWidget *parent = 0);
    ~DataTransfe();
    void getFolderSize(const QString & path, qint64 &size);
    void getFolderSize(const QDir & dir, qint64 &size);
    void bytesToSize(const qint64 & bytes, QString & size);

    const DataClient *clientForRow(int row) const;

protected:
    //void mouseMoveEvent(QMouseEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

signals:
    void updateControlSignal(bool);

public slots:
    void startUploadDataClientSlot();
    void updateProgressSlot(int percent);
    void updateUploadRateSlot(QString);
    void updateTransferStateSlot(QString);
    void updateCompletedSlot(QString);
    void updateRemainTimeSlot(QString);
    void updateTotalTimeSlot(QString);
    void transferDoneSlot(DataClient*);
    void transferNextSlot();

private slots:
    void create();
    void copyDir();
    void moveDir();
    void copyFiles();
    void moveFiles();
    void clearFiles();
    void copy();

    void stateChanged(QtFileCopier::State state);

private:
    int rowOfClient(DataClient *client) const;
    bool addClient(const QString &fileName, const QString &destinationFolder,
                    const QByteArray &resumeState = QByteArray());
    void saveSettings();

    QtFileCopier::CopyFlags currentFlags() const;
    void createDestDir(const QString &dest);
    QStringList sourceFiles() const;
    void removeDir(const QString &dir);

    //QtFileCopier *copier;

    QString createdDir;
    QString copiedDir;
    QString movedDir;
    QString copiedFiles;
    QString movedFiles;

    struct DataJob
    {
        QtFileCopier *copier;
        DataClient *dataClient;
        QString fileName;
        QString destinationDirectory;
    };

    QTimer *timer;
    QList<DataJob> jobs;
    int taskIndex;
    bool saveChanges;

};


// TorrentViewDelegate is used to draw the progress bars.
class DataViewDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    inline DataViewDelegate(QWidget *parent) : QItemDelegate(parent) {}

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
        int progress = qobject_cast<DataTransfe *>(parent())->clientForRow(index.row())->d_ptr->completedPercent;
        progressBarOption.progress = progress < 0 ? 0 : progress;
        progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);

        // Draw the progress bar onto the view.
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    }
};

#endif // DATATRANSFE_H
