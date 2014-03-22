#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <QTreeWidget>
#include <QMouseEvent>
#include <QItemDelegate>
#include <QApplication>

static QString bytesToSize(const qint64 &bytes);
static qint64 getFolderSize(const QString & path);

class FileClient;

class FileTransfer : public QTreeWidget
{
    Q_OBJECT
public:
    explicit FileTransfer(QWidget *parent = 0);
    ~FileTransfer();
    struct FileJob
    {
        FileClient* fileClient;
        QString fileName;
        QString destinationDirectory;
    };

    const FileClient *clientForRow(int row) const;
    QList<FileJob> fileJobs();

protected:
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

signals:
    
public slots:
    
private:
    bool addFile(const QString& filename);



    QList<FileJob> jobs;

};


// TorrentViewDelegate is used to draw the progress bars.
class FileTransferDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    inline FileTransferDelegate(QWidget *parent) : QItemDelegate(parent) {}

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
        int progress = 10;//qobject_cast<FileTransfer *>(parent())->clientForRow(index.row())->completedPercent;
        progressBarOption.progress = progress < 0 ? 0 : progress;
        progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);

        // Draw the progress bar onto the view.
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    }
};

#endif // FILETRANSFER_H
