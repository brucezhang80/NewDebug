#ifndef RECV_FILE_TREEWIDGET_H
#define RECV_FILE_TREEWIDGET_H

#include <QTreeWidget>
#include <QApplication>
#include <QItemDelegate>
#include <QFileSystemWatcher>
#include <QContextMenuEvent>

class RecvFileTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit RecvFileTreeWidget(QWidget *parent = 0);
    void retranslateUi();

    void addRow(const QString &path);
    void removeRow(int id);

    int count() const { return this->model()->rowCount(); }
    QString watchPath() const;
    void collectFiles();

signals:

private slots:
    void recvFileDone(const QList<QString> &fileLogList);
    void directoryChanged(const QString &path);
    void fileChanged(const QString &path);
    void openFile();
    void deleteFile();
    void refresh();

private:
    void initialize();
    void setConnect();

    QFileSystemWatcher          m_fsWatcher;
    bool                        m_isCollected;

protected:
    void contextMenuEvent(QContextMenuEvent *event);


};


class RecvFileItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    inline RecvFileItemDelegate(QWidget *parent) : QItemDelegate(parent) {}

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
        int progress = 0;//qobject_cast<RecvFileTreeWidget *>(parent())->clientForRow(index.row())->d_ptr->completedPercent;
        progressBarOption.progress = progress < 0 ? 0 : progress;
        progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);

        // Draw the progress bar onto the view.
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    }
};
#endif // RECV_FILE_TREEWIDGET_H
