#ifndef SEND_FILE_TREEWIDGET_H
#define SEND_FILE_TREEWIDGET_H

#include "qtfilecopier.h"
#include "send_file_client.h"

#include <QTreeWidget>
#include <QItemDelegate>
#include <QApplication>
#include <QFileInfo>
#include <QPushButton>

class QTimer;

//class SendFileClient;

class SendFileTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit SendFileTreeWidget(QWidget *parent = 0);
    ~SendFileTreeWidget();
    void retranslateUi();

    const SendFileClient *clientForRow(int row) const;
    void addFileClient();

    QStringList pathList()const;

signals:
    void updateHistoryLog(const QList<QString> &jobLog);
    void sendFileDone(const QList<QString> &jobLog);

public slots:
    void startSendSlot();
    void stopSendSlot();

private slots:
    void updateProgressSlot(const int &percent);
    void updateSendRateSlot(const QString &rate);
    void updateTransferStateSlot(const QString &state);
    void updateCompletedSlot(const QString &completed);
    void updateRemainTimeSlot(const QString &remianTime);
    void updateTotalTimeSlot(const QString &totalTime);

    void transferDoneSlot();
    void transferNextSlot();
    void transferAllDone();

    void stateChanged(QtFileCopier::State state);

    void clearDataJobs();
    void clearAllClient();

    void deleteRow();

private:
    void initialize();
    void setConnect();

    QString absolutePath( int row ) const;
    void addRow( const QStringList &columns );
    void removeRow();
    int rowCount() const;
    int rowOfClient(SendFileClient *client) const;
    QString fileName( int row ) const;
    QString sizeString( QFileInfo &fileInfo ) const;

    void addFile(const QString &path);
    void addFiles(const QStringList &pathList);

    void addDelBtn(QTreeWidgetItem *item);
    QtFileCopier::CopyFlags currentFlags() const;

    QPushButton     *m_delBtn;
    QStringList     m_pathList;
    struct SendJob
    {
        QtFileCopier    *copier;
        SendFileClient  *sendClient;
        int             id;
        QList<QString>  destinationList;
        QString         fileName;
        QString         size;
        QString         from;
        QString         time;
        QString         project;
    };
    QList<SendJob>      sendJobs;
    QList<QString>      m_recverInfo;

    QString             m_project;
    QTimer              *m_sendTimer;
    bool                m_bOneClientDone;
    int                 m_completedCounts;
};

class SendFileItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    inline SendFileItemDelegate(QWidget *parent) : QItemDelegate(parent) {}

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
        int progress = qobject_cast<SendFileTreeWidget *>(parent())->clientForRow(index.row())->d_ptr->completedPercent;
        progressBarOption.progress = progress < 0 ? 0 : progress;
        progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);

        // Draw the progress bar onto the view.
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    }
};

#endif // SEND_FILE_TREEWIDGET_H
